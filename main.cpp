#include "event.hpp"

#include "spacetimes/ads.hpp"

#include "sprinkler/sprinkler_builder.hpp"

#include "stats_gathering/stats_gatherer.hpp"

#include "utils/graph_dump.hpp"
#include "utils/sprinkle_dump.hpp"

int main(int argc, char * argv[])
{
    const auto d = 2;
    auto builder = SprinklerBuilder<d>();
    const auto R0 = 1;
    builder.buildAds(R0);
    // builder.buildMinkowski();

    auto bottom = Event<d>({-0.5, R0});
    auto top = Event<d>({0.5, R0});
    builder.buildCausalRegion(bottom, top);

    std::array<long double, 2 * d> bounds({-0.5, 0.5, R0, 0.5 + R0});
    builder.buildRectangularEnclosure(bounds);

    auto sprinkler = builder.getSprinkler();
    StatsGatherer<d> gatherer(sprinkler);

    gatherer.takeSamples(100, 1000, 9, 9);
    gatherer.dump(
        "/mnt/c/Projekty/Coding/C++/Sprinkling/data/gatherer/fix_minkowski2_half_dos/order_frac.csv",
        "/mnt/c/Projekty/Coding/C++/Sprinkling/data/gatherer/fix_minkowski2_half_dos/path_lengths.csv"
    );

    auto sprinkle = sprinkler.sprinkle(12500);
    sprinkle.addEvent(bottom);
    sprinkle.addEvent(top);

    CausalUtils::dumpSprinkleCsv(sprinkle, "/mnt/c/Projekty/Coding/C++/Sprinkling/data/gatherer/test_ads2_half_r0_1_12500/sprinkle.csv");

    auto dag = sprinkle.generateCausalSet();
    GraphUtils::dumpDagToCsv(dag, "/mnt/c/Projekty/Coding/C++/Sprinkling/data/gatherer/test_ads2_half_r0_1_12500/dag.csv");

    auto path = dag.findLongestPath();
    GraphUtils::dumpDagLongestPathToCsv(path, "/mnt/c/Projekty/Coding/C++/Sprinkling/data/gatherer/test_ads2_half_r0_1_12500/path.csv");

    CausalUtils::dumpDagLongestPathPointsToCsv(sprinkle, path, "/mnt/c/Projekty/Coding/C++/Sprinkling/data/gatherer/test_ads2_half_r0_1_12500/path_points.csv");

    return 0;
}
