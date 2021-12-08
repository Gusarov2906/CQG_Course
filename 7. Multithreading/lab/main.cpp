#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

#include <iomanip>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <memory>

#include <Windows.h>

struct Notification
{
   std::thread thread;
   std::string message;
   std::chrono::steady_clock::time_point wake;
};

std::mutex m_mutex;
std::condition_variable m_alarm;
std::vector<std::shared_ptr<Notification>> m_notifications;
std::vector<std::shared_ptr<Notification>> m_listedNotifications;

void New(int delay, std::string message)
{
   auto notif = std::make_shared<Notification>();

   {
      std::unique_lock<std::mutex> lock(m_mutex);
      m_notifications.push_back(notif);
   }

   notif->message = message;
   notif->wake = std::chrono::steady_clock::now() + std::chrono::seconds(delay);
   notif->thread = std::thread(
      [notif]() mutable
   {
      std::unique_lock<std::mutex> lock(m_mutex);
      const bool removed = m_alarm.wait_until(lock, notif->wake,
         [notif]()
      {
         auto iter = std::find(m_notifications.begin(), m_notifications.end(), notif);
         return iter == m_notifications.end(); // Wake up if removed
      });
      lock.unlock();

      if (!removed)
      {
         ::MessageBox(nullptr, notif->message.c_str(), "Message Box", 0);

         lock.lock();
         auto iter = std::find(m_notifications.begin(), m_notifications.end(), notif);
         if (iter != m_notifications.end())
            m_notifications.erase(iter);
      }
      notif->thread.detach();
   });

   std::cout << "New notification scheduled!" << std::endl;
}

void List()
{
   {
      std::unique_lock<std::mutex> lock(m_mutex);
      m_listedNotifications = m_notifications;
   }

   std::cout << "Scheduled notifications:" << std::endl;
   int index = 0;
   for (auto notif : m_listedNotifications)
   {
      const auto delay = std::chrono::duration_cast<std::chrono::seconds>(notif->wake - std::chrono::steady_clock::now()).count();
      std::cout << std::setw(4) << index + 1 << ".\t" << notif->message << " (in " << delay << " seconds)" << std::endl;
      ++index;
   }
}

void Cancel(int index)
{
   --index;
   if (index < m_listedNotifications.size())
   {
      auto notifToRemove = m_listedNotifications[index];

      {
         std::unique_lock<std::mutex> lock(m_mutex);
         auto iter = std::find(m_notifications.begin(), m_notifications.end(), notifToRemove);
         if (iter != m_notifications.end())
            m_notifications.erase(iter);
      }

      m_alarm.notify_all();
      m_listedNotifications[index] = nullptr;
      std::cout << "Notification with text \"" << notifToRemove->message << "\" was canceled" << std::endl;
   }
}

void Exit()
{
   {
      std::unique_lock<std::mutex> lock(m_mutex);
      m_notifications.clear();
      m_listedNotifications.clear();
   }
   m_alarm.notify_all();
}

enum class CommandType
{
   None,
   New,
   List,
   Cancel,
   Exit
};

struct Command
{
   CommandType type{};
   int delay{};
   std::string message;
   int index{};

   Command() = default;
   Command(CommandType type) : type(type) {}
   Command(CommandType type, int delay, const std::string& message) : type(type), delay(delay), message(message) {}
   Command(CommandType type, int index) : type(type), index(index) {}
};

Command ReadCommand()
{
   std::string input;
   std::getline(std::cin, input);

   std::stringstream parser(input);
   std::string command;
   if (parser >> command)
   {
      if (command == "exit" || command == "x")
         return Command(CommandType::Exit);
      else if (command == "list" || command == "l")
         return Command(CommandType::List);
      else if (command == "cancel" || command == "c")
      {
         int index = 0;
         if (parser >> index)
            return Command(CommandType::Cancel, index);
         else
         {
            std::cerr << "Usage: cancel index" << std::endl
                      << "   index : index of the item to remove" << std::endl;
         }
      }
      else if (command == "new" || command == "n")
      {
         int delay = 0;
         if (parser >> delay)
         {
            if (delay > 0)
            {
               std::string message;
               std::getline(parser, message);
               if (!message.empty())
                  message = message.substr(1);
               return Command(CommandType::New, delay, message);
            }
            else
               std::cerr << "Delay must be positive" << std::endl;
         }
         else
         {
            std::cerr << "Usage: new delay message" << std::endl
                      << "   delay   : positive delay in seconds" << std::endl
                      << "   message : message to show without quotes" << std::endl;
         }
      }
      else
      {
         std::cerr << "Unknown command" << std::endl;
      }
   }

   return Command(CommandType::None);
}

int main()
{
   std::cout
      << "Commands:" << std::endl
      << "   new <delay> <message>" << std::endl
      << "      Schedule a notification with given message and delay in seconds" << std::endl
      << "      delay   : positive delay in seconds" << std::endl
      << "      message : message to show without quotes" << std::endl
      << "   list" << std::endl
      << "      Show the list of active notifications" << std::endl
      << "   cancel <index>" << std::endl
      << "      Cancel active notification with given index" << std::endl
      << "      index   : index in the previously shown list" << std::endl
      << "   exit" << std::endl
      << "      Exit application" << std::endl;

   while (true)
   {
      std::cout << std::endl << "> ";
      const auto command = ReadCommand();
      switch (command.type)
      {
      case CommandType::None:
         continue;
      case CommandType::Exit:
         Exit();
         return 0;
      case CommandType::New:
         New(command.delay, command.message);
         break;
      case CommandType::List:
         List();
         break;
      case CommandType::Cancel:
         Cancel(command.index);
         break;
      default:
         assert(0);
      }
   }
   return 0;
}
