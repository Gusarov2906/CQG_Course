#define _CRT_SECURE_NO_WARNINGS

#include "TickerPlant.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

const int buf_size = 100;

// Fuction to convert time to string
std::string tickTimeToStr(const tm& tickTime)
{
    char buf[buf_size];
    strftime(buf, buf_size, "%X", &tickTime);
    std::string res = buf;
    return res;
}

// Function to get numeric param from std::cin
bool getNumericParam(int& param)
{
    std::cin >> param;
    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        std::cout << "Invalid input " << std::endl;
        return false;
    }
    return true;
}

int main()
{    
    std::cout.precision(8);

    // Initial initialization of parameters
    int timeInterval;
    MSG msg;
    std::vector<Tick*> barTicks;
    int countBars;
    bool isFirstTick = true;
    time_t firstTickTime;

    // Get correct params from user
    while (true)
    {
        std::cout << "Write time interval for bars: ";
        if (!getNumericParam(timeInterval))
            continue;
        std::cout << "Write count of bars: ";
        if (!getNumericParam(countBars))
            continue;
        if (timeInterval <= 0 || countBars < 1)
        {
            std::cout << "Bad start params.\n"
                << "Time interval must be equal or greater than 0.\n"
                << "Count of bars must be greater than 0." << std::endl;
            continue;
        }
        break;
    }

    // Start sending data
    StartFeed();

    // Data receiving and processing cycle
    while (true)
    {
        // Receive data
        GetMessage(&msg, NULL, 0, 0);

        // Get point of reference of time
        if (isFirstTick)
        {
            isFirstTick = false;
            firstTickTime = mktime(&reinterpret_cast<Tick*>(msg.lParam)->tickTime);
        }

        // Creating a bar based on incoming ticks for a time interval
        if (barTicks.size() > 0 &&
            (mktime(&reinterpret_cast<Tick*>(msg.lParam)->tickTime) - firstTickTime) >= timeInterval)
        {
            auto minMaxVal = std::minmax_element(barTicks.begin(), barTicks.end(), 
                [](const Tick* a, const Tick* b) { return a->tickPrice < b->tickPrice; });

            std::cout << "Bar closed [" << barTicks.front()->tickPrice << "; "
                                        << (*minMaxVal.first)->tickPrice << "; "
                                        << (*minMaxVal.second)->tickPrice << "; "
                                        << barTicks.back()->tickPrice << "]" << std::endl;

            // Freeing the memory of processed ticks
            for (Tick* tick : barTicks)
            {
                delete tick;
            }
            barTicks.clear();

            // The cycle of receiving and processing stop condition
            if (--countBars == 0)
                break;

            // Increasing the reference point by an interval
            firstTickTime += timeInterval;
        }
        
        // Adding data to the container for further processing
        barTicks.push_back(reinterpret_cast<Tick*>(msg.lParam));

        // Output of receiving tick data
        std::cout << barTicks.back()->tickPrice << " @ "
                  << tickTimeToStr(barTicks.back()->tickTime) 
                  << std::endl;
    }
    // Stop sending data
    StopFeed();
    return 0;
}