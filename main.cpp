#include "event.hpp"

#include "spacetimes/ads.hpp"

#include "sprinkler/sprinkler_builder.hpp"

#include "stats_gathering/stats_gatherer.hpp"

int main(int argc, char * argv[])
{
    const auto d = 4;
    auto builder = SprinklerBuilder<d>();
    const auto R0 = 1e-3;
    builder.buildAds(R0);

    auto bottom = Event<d>({-0.5, R0, 0, 0});
    auto top = Event<d>({0.5, R0, 0, 0});
    builder.buildCausalRegion(bottom, top);

    std::array<long double, 2 * d> bounds({-0.5, 0.5, R0, 0.5 + R0, -0.5, 0.5, -0.5, 0.5});
    builder.buildRectangularEnclosure(bounds);

    auto sprinkler = builder.getSprinkler();
    StatsGatherer<d> gatherer(sprinkler);

    gatherer.takeSamples(100, 2500, 24, 5);
    gatherer.dump(
        "/mnt/c/Projekty/Coding/C++/Sprinkling/data/gatherer/test_ads4_half_r0_1e-3_dos/order_frac.csv",
        "/mnt/c/Projekty/Coding/C++/Sprinkling/data/gatherer/test_ads4_half_r0_1e-3_dos/path_lengths.csv"
    );

    return 0;
}
