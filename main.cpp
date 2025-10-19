#include "event.hpp"

#include "spacetimes/de_sitter.hpp"
#include "spacetimes/minkowski.hpp"

#include "sprinkler/sprinkler_builder.hpp"

#include "utils/causal_functions.hpp"
#include "utils/graph_dump.hpp"
#include "utils/sprinkle_dump.hpp"

#include <iostream>

int main(int argc, char * argv[])
{
    const auto PI = 3.141592653589793238462643383279502884L;

    // Wow...
    const auto d = 2;
    auto builder = SprinklerBuilder<d>();
    const auto R0 = 0.01;
    builder.buildAds(R0);
    // auto spacetime = builder.buildMinkowski();
    // auto spacetime = builder.buildDeSitter({-0.45 * PI, 0.45 * PI, R0, 0.45 * PI + R0});

    std::cout << "Made Spacetime\n";

    auto bottom = Event<d>({-0.45 * PI, R0});
    auto top = Event<d>({0.45 * PI, R0});

    // std::array<long double, 2> extension({R0, 2 + R0});

    builder.buildCausalRegion(bottom, top);
    // auto region = builder.buildExtendedCausalRegion(bottom, top, extension, 1);

    // auto region = builder.buildRectangularRegion({-0.45 * PI, 0.45 * PI, - PI, PI});

    // auto r = 1.0;
    // auto L = 1.0;
    // auto cylinderCentre = Event<d>({0, R0 + r});
    // auto sphereCentre = Event<d>({0, R0 + 0.5 * r});

    // auto region = builder.buildCylinderRegion(0, cylinderCentre, L, r);
    // auto region = builder.buildSphericalRegion(sphereCentre, r);

    std::cout << "Made Region\n";

    std::array<long double, 2 * d> bounds({-0.45 * PI, 0.45 * PI, R0, 0.45 * PI + R0});
    // std::array<long double, 2 * d> bounds({-L, L, R0, R0 + 2 * r, -r, r});
    // std::array<long double, 2 * d> bounds({-r, r, R0, R0 + 2*r, -r, r});
    // std::array<long double, 2*d> bounds({-r, r, -r, r, -r, r});

    builder.buildRectangularEnclosure(bounds);
    std::cout << "Made Enclosure\n";

    auto sprinkler = builder.getSprinkler();
    // std::cout << "Got Sprinkler\n";

    std::cout << "Values good!\n";
    auto sprinkle = sprinkler.sprinkle(1000);
    std::cout << "Sprinkled.";
    CausalUtils::dumpSprinkleCsv(sprinkle, "/mnt/c/Projekty/Coding/C++/Sprinkling/data/refactor/test_ds_half_2_1000.csv");

    auto graph = sprinkle.generateCausalSet();

    GraphUtils::dumpDagToCsv(graph, "/mnt/c/Projekty/Coding/C++/Sprinkling/data/refactor/dag_ds_half_2_1000.csv");

    auto longestPath = graph.findLongestPath();
    GraphUtils::dumpDagLongestPathToCsv(longestPath, "/mnt/c/Projekty/Coding/C++/Sprinkling/data/refactor/path_ds_half_2_1000.csv");

    return 0;
}
