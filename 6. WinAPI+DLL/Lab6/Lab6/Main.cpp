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

int main()
{    
    std::cout.precision(8);

    // Initial initialization of parameters
    int timeInterval;
    MSG msg;
    std::vector<Tick*> barTicks;
    int countBars;

    // Get params from user
    std::cout << "Write time interval for bars: ";
    std::cin >> timeInterval;
    std::cout << "Write count of bars: ";
    std::cin >> countBars;

    // Start sending data
    StartFeed();

    // Data receiving and processing cycle
    while (true)
    {
        // Receive data
        GetMessage(&msg, NULL, 0, 0);

        // Creating a bar based on incoming ticks for a time interval
        if (barTicks.size() > 0 &&
            (mktime(&reinterpret_cast<Tick*>(msg.lParam)->tickTime) - mktime(&barTicks.front()->tickTime)) >= timeInterval)
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
            if (countBars == 1)
                break;
            else
                countBars--;
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