#include "directed_graph.hpp"

//---------------------------------------------------------------------------------------------------------------------

DirectedGraph::DirectedGraph(unsigned int numNodes)
{
    for (unsigned int i = 0; i < numNodes; i++)
    {
        addNode(i);
    }
}

//---------------------------------------------------------------------------------------------------------------------

void DirectedGraph::addNode(nodeId node)
{
    if (nodes.find(node) != nodes.end())
    {
        std::__throw_invalid_argument("Trying to insert node which already exists.");
    }

    nodes.insert(node);
    adjacencyList[node] = std::set<nodeId>({});
}

//---------------------------------------------------------------------------------------------------------------------

void DirectedGraph::removeNode(nodeId node)
{
    if (nodes.find(node) == nodes.end())
    {
        std::__throw_invalid_argument("Trying to remove node which does not exist.");
    }

    nodes.erase(node);
    adjacencyList.erase(node);
}

//---------------------------------------------------------------------------------------------------------------------

void DirectedGraph::addEdge(nodeId from, nodeId to)
{
    if (nodes.find(from) == nodes.end())
    {
        std::__throw_invalid_argument("Trying to insert edge from nodes for a parent that does not exist.");
    }

    if (nodes.find(to) == nodes.end())
    {
        std::__throw_invalid_argument("Trying to insert edge to a node that does not exist.");
    }

    auto & reverseEdges = adjacencyList.at(to);

    if (reverseEdges.find(from) != reverseEdges.end())
    {
        std::__throw_invalid_argument("Trying to insert edge between nodes that are connected in the reverse direction.");
    }

    auto & destNodes = adjacencyList.at(from);

    if (destNodes.find(to) != destNodes.end())
    {
        std::__throw_invalid_argument("Trying to insert edge that already exists.");
    }

    destNodes.insert(to);
}

//---------------------------------------------------------------------------------------------------------------------

void DirectedGraph::removeEdge(nodeId from, nodeId to)
{
    if (nodes.find(from) == nodes.end())
    {
        std::__throw_invalid_argument("Trying to remove edge from nodes for a parent that does not exist.");
    }

    if (nodes.find(to) == nodes.end())
    {
        std::__throw_invalid_argument("Trying to remove edge to a node that does not exist.");
    }

    auto & destNodes = adjacencyList.at(from);

    if (destNodes.find(to) != nodes.end())
    {
        std::__throw_invalid_argument("Trying to remove edge that does not exist.");
    }

    destNodes.erase(to);
}
