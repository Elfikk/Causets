#pragma once

#include "event.hpp"

#include <array>
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
        for (int i = 0; i < d; i++)
        {
            randomNums[i] = uniform(rd);
        }
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
    template<int d>
    std::optional<Event<d>> minkowskiRegionSprinkleEvent(Region<d> & sprinkleRegion, RectangularRegion<d> & enclosingRegion)
    {
        const auto randomNums = generateRandomNumbers<d>();
        std::array<double, d> coords;
        for (int i = 0; i < d; i++)
        {
            coords[i] = linear_interpolate(
                randomNums[i],
                enclosingRegion.getLowerBound(i),
                enclosingRegion.getUpperBound(i)
            );
        }

        Event<d> event(coords);
        return sprinkleRegion.isInside(event) ? event : std::nullopt;
    };

//---------------------------------------------------------------------------------------------------------------------

    template<int d>
    std::optional<Event<d>> adsRegionSprinkleEvent(Region<d> & sprinkleRegion, RectangularRegion<d> & enclosingRegion)
    {
        const auto randomNums = SprinkleUtils::generateRandomNumbers<d>();
        // Now map random numbers to coordinates within enclosing region
        std::array<double, d> coords;
        for (int i = 0; i < d; i++)
        {
            coords[i] = linear_interpolate(
                randomNums[i],
                enclosingRegion.getLowerBound(i),
                enclosingRegion.getUpperBound(i)
            );
        }
        const auto R0 = sprinkleRegion.getR0();
        coords[1] = R0 / (1 - x)**(1 / d-1);

        Event<d> event(coords);
        return sprinkleRegion.isInside(event) ? event : std::nullopt;
    };
};
