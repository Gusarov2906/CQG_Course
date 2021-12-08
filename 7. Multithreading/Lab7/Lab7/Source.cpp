#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>
#include <WinUser.h>
#include <winnt.h>
#include <vector>
#include <ctime>
#include <thread>
#include <mutex>
#include <algorithm>

#include "Message.h"

std::vector<std::shared_ptr<Message>> messageList;
std::mutex showMutex;
std::vector<std::shared_ptr<std::thread>> threads;


//Function to get id of message from MessageList by index of vector(starts wuth 0)
uint32_t getIdInMessageList(const int& index)
{
    if (index < messageList.size())
    {
        return messageList[index]->getId();
    }
    else
    {
        throw std::string("Index is not correct!");
    }
}

//Function to get index of vector(starts wuth 0) from MessageList by id of message
int getIndexByIdFromMessageList(const uint32_t& id)
{
    for (int i = 0; i < messageList.size(); i++)
    {
        if (messageList[i]->getId() == id)
        {
            return i;
        }
    }
    throw std::string("Id is not correct!");
}

//Function to display messagebox with text from msg
void Show(std::shared_ptr<Message> msg)
{
    std::string name = msg->getName();
    LPCSTR text = name.c_str();
    LPCSTR title = "Notification";
    MessageBoxA(0, text, title, MB_OK);
}

//Function to show messagebox after delay
void DelayedShow(std::shared_ptr<Message> msg)
{
    //delay calculates from msg
    int delay = msg->calculateRemainingTime();
    
    //waith remaining time
    while (delay)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        delay--;
        //if task canceled delete it from messageList and exit from function
        if (msg->getQuitFlag())
        {
            messageList.erase(messageList.begin() + getIndexByIdFromMessageList(msg->getId()));
            //std::cout << "Notification with text \"" << msg->getName() << "\" was cancelled" << std::endl;
            return;
        }
    }
    //delete from scheldure
    try
    {
        messageList.erase(messageList.begin() + getIndexByIdFromMessageList(msg->getId()));
    }
    catch (std::string err)
    {
        std::cout << err << std::endl;
        return;
    }
    //show MessageBox
    Show(msg);
}

//Function to add to scheldure notification
void New(int delay, std::string message)
{
    //create timePoint with with extra time(delay) from the current one
    std::chrono::system_clock::time_point timeToShow = std::chrono::system_clock::from_time_t(time(NULL)+delay);
    //added to messageList
    messageList.push_back(std::shared_ptr<Message>(new Message(message, timeToShow)));
    int id;
    try
    {
        id = getIdInMessageList(messageList.size() - 1);
    }
    catch(std::string err)
    {
        std::cout << err << std::endl;
        std::cout << "Notification not scheldured!" << std::endl;
        return;
    }
    //add last element messageList to thread for show after delay
    threads.push_back(std::shared_ptr<std::thread>(new std::thread(DelayedShow, messageList[messageList.size() - 1])));
    threads.back()->detach();
    std::cout << "New notification scheldured!" << std::endl;
}

//Function to display all scheldured notifications
void List()
{
    if (messageList.size() > 0)
    {
        for (uint16_t i = 0; i < messageList.size(); i++)
        {
            //display indexes starts from 1
            std::cout << "\t" << i + 1 << ".\t" + messageList[i]->getReport() << std::endl;
            //memorize the index in the list
            messageList[i]->setIndexInList(i + 1);
        }
    }
    else
    {
        std::cout << "There is notifications in scheldure." << std::endl;
    }
}

//Function to cancel notification
void Cancel(int index)
{
    for (std::shared_ptr<Message> msg : messageList)
    {
        //try to get msg by index in list created by last list usage
        if (msg->getIndexInList() == index)
        {
            std::cout << "Notification with text \"" << msg->getName() << "\" was cancelled" << std::endl;
            msg->setQuitFlag(true);
            return;
        }
    }
    std::cout << "There is no notification with this index in scheldure.\nPlease use \"list\" to find index before cancel!" << std::endl;
}

//Function to stop all notifications and exit from programm
void Exit()
{
    for (std::shared_ptr<Message> msg: messageList)
    {
        msg->setQuitFlag(true);
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "All threads finished!" << std::endl;
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