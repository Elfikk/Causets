#include "Graphs/DirectedGraph.hpp"

int main(int argc, char * argv[])
{
    auto graph = DirectedGraph();

    // Make a crown
    graph.addNode(1);
    graph.addNode(2);
    graph.addNode(3);
    graph.addNode(4);
    graph.addNode(5);
    graph.addNode(6);

    // 1 -> 4, 5
    graph.addEdge(1, 4);
    graph.addEdge(1, 5);

    // 2 -> 4, 6
    graph.addEdge(2, 4);
    graph.addEdge(2, 6);

    // 3 -> 5, 6
    graph.addEdge(3, 5);
    graph.addEdge(3, 6);

    // Wow...
}
