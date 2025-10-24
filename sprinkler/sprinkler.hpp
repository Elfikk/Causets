#pragma once

#include "event.hpp"

#include "regions/rectangular.hpp"
#include "regions/region.hpp"

#include "spacetimes/spacetime.hpp"

#include "sprinkler/sprinkle.hpp"
#include "sprinkler/sprinkle_strategies.hpp"

#include <functional>
#include <iostream>
#include <memory>

template<int d>
class Sprinkle;

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class Sprinkler
{
public:
    Sprinkler(
        std::function<std::optional<Event<d>>()> sprinkleFunc,
        std::function<CausalRelation(const Event<d> &, const Event<d> &)> relationFunc,
        std::vector<Event<d>> specialPoints = {}
    )
    :
    sprinkleFunction(sprinkleFunc),
    relationFunction(relationFunc),
    specialPathPoints(specialPoints)
    {};

    Sprinkle<d> sprinkle(int points);

private:
    std::function<std::optional<Event<d>>()> sprinkleFunction;
    std::function<CausalRelation(const Event<d> &, const Event<d> &)> relationFunction;
    std::vector<Event<d>> specialPathPoints;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
Sprinkle<d> Sprinkler<d>::sprinkle(const int points)
{
    Sprinkle<d> sprinkle(relationFunction, specialPathPoints);
    int generatedPoints = 0;

    while (generatedPoints < points)
    {
        auto potentialPoint = sprinkleFunction();
        if (potentialPoint.has_value())
        {
            generatedPoints++;
            sprinkle.addEvent(*potentialPoint);
        }
    }

    return sprinkle;
}
