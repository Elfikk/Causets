#pragma once

#include "event.hpp"

#include "spacetimes/ads.hpp"
#include "spacetimes/de_sitter.hpp"
#include "spacetimes/minkowski.hpp"
#include "spacetimes/spacetime.hpp"

#include "regions/region.hpp"

#include <array>
#include <cmath>
#include <optional>
#include <random>

//---------------------------------------------------------------------------------------------------------------------

namespace SprinkleUtils
{
template<int d>
std::array<long double, d> generateRandomNumbers()
{
    // Good old urandom
    std::random_device rd("/dev/urandom");
    std::uniform_real_distribution<> uniform(0.0, 1.0);
    std::array<long double, d> randomNums;
    for (int i = 0; i < d; i++)
    {
        randomNums[i] = uniform(rd);
    }
    return randomNums;
}

long double linearInterpolate(long double x, long double a, long double b)
{
    return (b - a) * x + a;
}
}

//---------------------------------------------------------------------------------------------------------------------

namespace SprinkleStrategy
{

//---------------------------------------------------------------------------------------------------------------------

template<int d>
std::optional<Event<d>> doSprinkleEvent(
    Region<d> * sprinkleRegion,
    RectangularRegion<d> & enclosingRegion,
    minkowski_spacetime)
{
    const auto randomNums = SprinkleUtils::generateRandomNumbers<d>();
    std::array<long double, d> coords;
    for (int i = 0; i < d; i++)
    {
        coords[i] = SprinkleUtils::linearInterpolate(
            randomNums[i],
            enclosingRegion.getLowerBound(i),
            enclosingRegion.getUpperBound(i)
        );
    }

    Event<d> event(coords);
    return sprinkleRegion->isInside(event) ? std::optional(event) : std::nullopt;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
std::optional<Event<d>> minkowskiEventSprinkle(
    Region<d> * sprinkleRegion,
    RectangularRegion<d> & enclosingRegion
)
{
    return doSprinkleEvent(sprinkleRegion, enclosingRegion, minkowski_spacetime());
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
std::optional<Event<d>> doSprinkleEvent(
    Region<d> * sprinkleRegion,
    RectangularRegion<d> & enclosingRegion,
    ads_spacetime)
{
    const auto randomNums = SprinkleUtils::generateRandomNumbers<d>();
    // Now map random numbers to coordinates within enclosing region
    std::array<long double, d> coords;
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
    }

    Event<d> event(coords);
    return sprinkleRegion->isInside(event) ? std::optional(event) : std::nullopt;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
std::optional<Event<d>> adsEventSprinkle(
    Region<d> * sprinkleRegion,
    RectangularRegion<d> & enclosingRegion
)
{
    return doSprinkleEvent(sprinkleRegion, enclosingRegion, ads_spacetime());
}
//---------------------------------------------------------------------------------------------------------------------

long double deSitterTimeFromRandom(long double r, long double z0, long double z1, long double zInf, long double d)
{
    const auto prePowerDenominator = r * pow(zInf - z0, d-1) + (1 - r) * pow((zInf - z1), d-1);
    const auto numerator = (zInf - z1) * (zInf - z0);
    return zInf - numerator / pow(prePowerDenominator, 1.0 / (d - 1));
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
std::optional<Event<d>> doSprinkleEvent(
    Region<d> * sprinkleRegion,
    RectangularRegion<d> & enclosingRegion,
    de_sitter_spacetime)
{
    const auto randomNums = SprinkleUtils::generateRandomNumbers<d>();
    std::array<long double, d> coords;
    for (int i = 0; i < d; i++)
    {
        if (i != 0)
        {
            coords[i] = SprinkleUtils::linearInterpolate(
                randomNums[i],
                enclosingRegion.getLowerBound(i),
                enclosingRegion.getUpperBound(i)
            );
        }
        else
        {
            coords[i] = deSitterTimeFromRandom(
                randomNums[0],
                enclosingRegion.getLowerBound(i),
                enclosingRegion.getUpperBound(i),
                1.0,
                d
            );
        }
    }
    Event<d> event(coords);
    return sprinkleRegion->isInside(event) ? std::optional(event) : std::nullopt;
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
std::optional<Event<d>> deSitterEventSprinkle(
    Region<d> * sprinkleRegion,
    RectangularRegion<d> & enclosingRegion
)
{
    return doSprinkleEvent(sprinkleRegion, enclosingRegion, de_sitter_spacetime());
}

//---------------------------------------------------------------------------------------------------------------------

template<int d, typename SpacetimeT>
std::optional<Event<d>> sprinkleEvent(Region<d> * sprinkleRegion, RectangularRegion<d> & enclosingRegion)
{
    return doSprinkleEvent(sprinkleRegion, enclosingRegion, typename spacetime_traits<SpacetimeT>::spacetime_class());
}

};
