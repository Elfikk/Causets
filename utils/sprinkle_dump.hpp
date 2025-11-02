#pragma once

#include "event.hpp"

#include "sprinkler/sprinkle.hpp"

#include <iostream>
#include <fstream>
#include <string>

//---------------------------------------------------------------------------------------------------------------------

namespace CausalUtils
{

// Dump Event into a CSV Format
template<int d>
void dumpEventCsv(std::ofstream & stream, const Event<d> & event)
{
    stream << event[0];
    for (unsigned int i = 1; i < d; i++)
    {
        stream << ",";
        stream << event[i];
    }
}

//---------------------------------------------------------------------------------------------------------------------

// Function For Dumping a Sprinkle to a CSV
template<int d>
void dumpSprinkleCsv(Sprinkle<d> & sprinkle, const std::string filename)
{
    auto events = sprinkle.getEvents();
    std::ofstream dumpFile(filename);
    for (const auto & event : events)
    {
        dumpEventCsv(dumpFile, event);
        dumpFile << '\n';
    }
    dumpFile.close();
}

//---------------------------------------------------------------------------------------------------------------------

// Dump Longest Path Points to CSV
template<int d>
void dumpDagLongestPathPointsToCsv(Sprinkle<d> & sprinkle, const std::vector<unsigned int> & path, const std::string filename)
{
    auto events = sprinkle.getEvents();
    std::ofstream dumpFile(filename);
    for (const auto & id : path)
    {
        dumpEventCsv(dumpFile, events[id]);
        dumpFile << '\n';
    }
    dumpFile.close();
}

}
