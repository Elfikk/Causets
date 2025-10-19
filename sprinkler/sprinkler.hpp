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
        std::function<CausalRelation(const Event<d> &, const Event<d> &)> relationFunc
    )
    :
    sprinkleFunction(sprinkleFunc),
    relationFunction(relationFunc)
    {};

    Sprinkle<d> sprinkle(int points);

private:
    std::function<std::optional<Event<d>>()> sprinkleFunction;
    std::function<CausalRelation(const Event<d> &, const Event<d> &)> relationFunction;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
Sprinkle<d> Sprinkler<d>::sprinkle(const int points)
{
    Sprinkle<d> sprinkle(relationFunction);
    int generatedPoints = 0;

    while (generatedPoints < points)
    {
        auto potentialPoint = sprinkleFunction();
        if (potentialPoint.has_value())
        {
            generatedPoints++;
            sprinkle.addEvent(*potentialPoint);
        }
        std::cout << "Number of generated points: " << generatedPoints << "\n";
    }

    return sprinkle;
}
