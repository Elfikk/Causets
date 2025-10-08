#pragma once

#include "event.hpp"

#include "sprinkler/sprinkle.hpp"

#include <iostream>
#include <fstream>
#include <string>

namespace CausalUtils
{

// Dump Event into a CSV Format
template<int d>
void dumpEventCsv(std::ofstream & stream, const Event<d> & event)
{
    for (unsigned int i = 0; i < d; i++)
    {
        auto component = event[i];
        stream << component << ",";
    }
}

//---------------------------------------------------------------------------------------------------------------------

// Function For Dumping a Sprinkle to a CSV
template<int d, typename SpacetimeT>
void dumpSprinkleCsv(Sprinkle<d, SpacetimeT> & sprinkle, const std::string filename)
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

}
