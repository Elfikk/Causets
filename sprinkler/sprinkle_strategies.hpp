#pragma once

#include "event.hpp"

#include "spacetimes/ads.hpp"
#include "spacetimes/minkowski.hpp"
#include "spacetimes/spacetime.hpp"

#include <array>
#include <iostream>
#include <optional>
#include <random>

//---------------------------------------------------------------------------------------------------------------------

namespace SprinkleUtils
{
template<int d>
std::array<double, d> generateRandomNumbers()
{
    // Good old urandom
    std::random_device rd("/dev/urandom");
    std::uniform_real_distribution<> uniform(0.0, 1.0);
    std::array<double, d> randomNums;
    std::cout << "Random Num:\n";
    for (int i = 0; i < d; i++)
    {
        randomNums[i] = uniform(rd);
        std::cout << randomNums[i] << ",";
    }
    std::cout << "\n";
    return randomNums;
}

double linearInterpolate(double x, double a, double b)
{
    return (b - a) * x + a;
}
}

//---------------------------------------------------------------------------------------------------------------------

namespace SprinkleStrategy
{

//---------------------------------------------------------------------------------------------------------------------

template<int d, typename RegionT>
std::optional<Event<d>> doSprinkleEvent(RegionT & sprinkleRegion, RectangularRegion<d> & enclosingRegion, minkowski_spacetime)
{
    const auto randomNums = SprinkleUtils::generateRandomNumbers<d>();
    std::cout << "Coordinates:\n";
    std::array<double, d> coords;
    for (int i = 0; i < d; i++)
    {
        coords[i] = SprinkleUtils::linearInterpolate(
            randomNums[i],
            enclosingRegion.getLowerBound(i),
            enclosingRegion.getUpperBound(i)
        );
        std::cout << coords[i] << ",";
    }
    std::cout << "\n";

    Event<d> event(coords);
    return sprinkleRegion.isInside(event) ? std::optional(event) : std::nullopt;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d, typename RegionT>
std::optional<Event<d>> doSprinkleEvent(RegionT & sprinkleRegion, RectangularRegion<d> & enclosingRegion, ads_spacetime)
{
    const auto randomNums = SprinkleUtils::generateRandomNumbers<d>();
    std::cout << "Coordinates:\n";
    // Now map random numbers to coordinates within enclosing region
    std::array<double, d> coords;
    const auto R1 = enclosingRegion.getUpperBound(1);
    const auto R0 = enclosingRegion.getLowerBound(1);
    const auto R0_pow = pow(R0, 1 - d);
    const auto R1_pow = pow(R1, 1 - d);
    for (int i = 0; i < d; i++)
    {
        if (i != 1)
        {
            coords[i] = SprinkleUtils::linearInterpolate(
                randomNums[i],
                enclosingRegion.getLowerBound(i),
                enclosingRegion.getUpperBound(i)
            );
        }
        else
        {
            coords[i] = pow(R0_pow - randomNums[i] * (R0_pow - R1_pow), 1.0 / (1.0 - d));
        }
        std::cout << coords[i] << ",";
    }
    std::cout << "\n";

    Event<d> event(coords);
    std::cout << "Is inside " << sprinkleRegion.isInside(event) << "\n";
    return sprinkleRegion.isInside(event) ? std::optional(event) : std::nullopt;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d, typename SpacetimeT, typename RegionT>
std::optional<Event<d>> sprinkleEvent(RegionT sprinkleRegion, RectangularRegion<d> & enclosingRegion)
{
    return doSprinkleEvent(sprinkleRegion, enclosingRegion, typename spacetime_traits<SpacetimeT>::spacetime_class());
}

};
