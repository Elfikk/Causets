// #include "graphs/DirectedGraph.hpp"

#include "event.hpp"

#include "spacetimes/minkowski.hpp"

#include "sprinkler/sprinkler_builder.hpp"

#include "utils/causal_functions.hpp"

#include <iostream>

int main(int argc, char * argv[])
{
    // auto graph = DirectedGraph(7);
    // graph.removeNode(0);

    // // 1 -> 4, 5
    // graph.addEdge(1, 4);
    // graph.addEdge(1, 5);

    // // 2 -> 4, 6
    // graph.addEdge(2, 4);
    // graph.addEdge(2, 6);

    // // 3 -> 5, 6
    // graph.addEdge(3, 5);
    // graph.addEdge(3, 6);

    // Wow...

    auto builder = SprinklerBuilder<2>();
    builder.buildAds(0.01);

    auto bottom = Event<2>({-1, 0});
    auto top = Event<2>({1, 0});

    builder.buildCausalRegion(bottom, top);

    std::array<double, 4> bounds({-1, 1, -1, 1});

    builder.buildRectangularEnclosure(bounds);

    auto sprinkler = builder.getSprinkler();

    if (sprinkler.canSprinkle())
    {
        auto sprinkle = sprinkler.sprinkle(10);
        auto graph = sprinkle.generateCausalSet(sprinkler.getSpacetime());

        std::cout << "\nAnything?";
    }

    std::cout << "\nNo?";

    return 0;
}
