#pragma once

#include <map>
#include <set>
#include <vector>

typedef unsigned int nodeId;

class DirectedGraph
{
public:
    DirectedGraph() {}
    DirectedGraph(unsigned int numNodes);

    void addNode(nodeId node);
    void removeNode(nodeId node);

    void addEdge(nodeId from, nodeId to);
    void removeEdge(nodeId from, nodeId to);

private:
    std::set<nodeId> nodes;
    std::map<nodeId, std::set<nodeId>> adjacencyList;
};
