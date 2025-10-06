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
        const auto randomNums = SprinkleUtils::generateRandomNumbers<d>();
        std::array<double, d> coords;
        for (int i = 0; i < d; i++)
        {
            coords[i] = SprinkleUtils::linearInterpolate(
                randomNums[i],
                enclosingRegion.getLowerBound(i),
                enclosingRegion.getUpperBound(i)
            );
        }

        Event<d> event(coords);
        return sprinkleRegion.isInside(event) ? std::optional(event) : std::nullopt;
    };

//---------------------------------------------------------------------------------------------------------------------

    template<int d>
    std::optional<Event<d>> adsRegionSprinkleEvent(Region<d> & sprinkleRegion, RectangularRegion<d> & enclosingRegion)
    {
        const auto randomNums = SprinkleUtils::generateRandomNumbers<d>();
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
        }

        Event<d> event(coords);
        return sprinkleRegion.isInside(event) ? std::optional(event) : std::nullopt;
    };
};
