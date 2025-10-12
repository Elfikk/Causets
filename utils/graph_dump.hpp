#pragma once

#include <fstream>
#include <set>
#include <string>
#include <vector>

class DirectedGraph;

namespace GraphUtils
{
    // Dump Single Nodes from Adjacency List to CSV Stream
    // TODO - This should not be dependent on the underlying data structure within Directed Graph
    void dumpNodeCsv(std::ofstream & stream, const std::set<unsigned int> & event);

    // Dump Graph to CSV
    void dumpDagToCsv(const DirectedGraph & dag, const std::string filename);

    // Dump Longest Path to CSV
    void dumpDagLongestPathToCsv(const std::vector<unsigned int> & path, const std::string filename);
}
