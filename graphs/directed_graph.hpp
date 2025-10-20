#pragma once

#include <map>
#include <set>
#include <vector>

typedef unsigned int nodeId;

// This is an unweighted directed graph actually.
class DirectedGraph
{
public:
    DirectedGraph() {}
    DirectedGraph(unsigned int numNodes);

    void addNode(nodeId node);
    void removeNode(nodeId node);

    void addEdge(nodeId from, nodeId to);
    void removeEdge(nodeId from, nodeId to);

    std::size_t getNumNodes() const { return nodes.size(); }
    std::size_t getNumEdges() const { return edgeNum; }

    std::map<nodeId, std::set<nodeId>> getAdjacencyList() const { return adjacencyList; }

    std::vector<nodeId> findLongestPath();

private:
    void findTopologicalOrder();
    void dfsUtil(nodeId, std::set<nodeId> & visitedNodes);

    std::set<nodeId> nodes;
    std::map<nodeId, std::set<nodeId>> adjacencyList;

    std::vector<nodeId> topologicalOrder;

    std::size_t edgeNum = 0;
};
