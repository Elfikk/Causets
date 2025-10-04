#pragma once

#include <fstream>
#include <set>
#include <string>

class DirectedGraph;

namespace GraphUtils
{
    // Dump Single Nodes from Adjacency List to CSV Stream
    // TODO - This should not be dependent on the underlying data structure within Directed Graph
    void dumpNodeCsv(std::ofstream & stream, const std::set<unsigned int> & event);

    // Dump Graph to CSV
    void dumpDagToCsv(const DirectedGraph & dag, const std::string filename);
}
