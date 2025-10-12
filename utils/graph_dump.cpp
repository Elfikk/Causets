#include "graph_dump.hpp"

#include "graphs/directed_graph.hpp"

//---------------------------------------------------------------------------------------------------------------------

void GraphUtils::dumpNodeCsv(std::ofstream & stream, const std::set<nodeId> & nodes)
{
    for (const auto node : nodes)
    {
        stream << ",";
        stream << node;
    }
}

//---------------------------------------------------------------------------------------------------------------------

void GraphUtils::dumpDagToCsv(const DirectedGraph & dag, const std::string filename)
{
    const auto list = dag.getAdjacencyList();
    std::ofstream nodeFile(filename);
    for (nodeId i = 0; i < dag.getNumNodes(); i++)
    {
        nodeFile << i;
        dumpNodeCsv(nodeFile, list.at(i));
        nodeFile << "\n";
    }
    nodeFile.close();
}

//---------------------------------------------------------------------------------------------------------------------

void GraphUtils::dumpDagLongestPathToCsv(const std::vector<unsigned int> & path, const std::string filename)
{
    if (path.size() < 2)
    {
        return; // Should really throw an exception
    }
    std::ofstream nodeFile(filename);
    nodeFile << path[0];
    for (size_t i = 1; i < path.size(); i++)
    {
        nodeFile << ",";
        nodeFile << path[i];
    }
    nodeFile.close();
}
