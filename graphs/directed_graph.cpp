#include "directed_graph.hpp"

#include <algorithm>

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
        std::__throw_invalid_argument(
            "Trying to insert edge between nodes that are connected in the reverse direction."
        );
    }

    auto & destNodes = adjacencyList.at(from);

    if (destNodes.find(to) != destNodes.end())
    {
        std::__throw_invalid_argument("Trying to insert edge that already exists.");
    }

    destNodes.insert(to);
    edgeNum += 1;
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
    edgeNum -= 1;
}

//---------------------------------------------------------------------------------------------------------------------

void DirectedGraph::dfsUtil(nodeId id, std::set<nodeId> & visitedNodes)
{
    visitedNodes.insert(id);
    for (auto i : adjacencyList[id])
    {
        if (visitedNodes.find(i) == visitedNodes.end())
        {
            dfsUtil(i, visitedNodes);
        }
    }
    topologicalOrder.push_back(id);
}

//---------------------------------------------------------------------------------------------------------------------

void DirectedGraph::findTopologicalOrder()
{
    std::set<nodeId> visited({});

    for (nodeId i = 0; i < getNumNodes(); i++)
    {
        if (visited.find(i) == visited.end())
        {
            dfsUtil(i, visited);
        }
    }
    std::reverse(topologicalOrder.begin(), topologicalOrder.end());
}

//---------------------------------------------------------------------------------------------------------------------

std::vector<nodeId> DirectedGraph::findLongestPath()
{
    if (topologicalOrder.size() == 0)
    {
        findTopologicalOrder();
    }

    std::map<nodeId, std::vector<nodeId>> longestPathToNodes;
    for (nodeId node = 0; node < getNumNodes(); node++)
    {
        longestPathToNodes[node] = {};
    }

    size_t maxLength = 0;
    nodeId maxId = 0;
    for (auto nodeId : topologicalOrder)
    {
        for (auto otherId : adjacencyList[nodeId])
        {
            if (longestPathToNodes[otherId].size() <= longestPathToNodes[nodeId].size() + 1)
            {
                longestPathToNodes[otherId] = longestPathToNodes[nodeId];
                longestPathToNodes[otherId].push_back(nodeId);
                if (longestPathToNodes[otherId].size() > maxLength)
                {
                    maxLength = longestPathToNodes[otherId].size();
                    maxId = otherId;
                }
            }
        }
    }

    longestPathToNodes[maxId].push_back(maxId);
    return longestPathToNodes[maxId];
}
