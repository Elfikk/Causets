#pragma once

#include "event.hpp"

#include "regions/rectangular.hpp"
#include "regions/region.hpp"

#include "spacetimes/spacetime.hpp"

#include "sprinkler/sprinkle.hpp"
#include "sprinkler/sprinkle_strategies.hpp"

#include <functional>
#include <iostream>
#include <optional>
#include <memory>

template<int d>
class Sprinkle;

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class Sprinkler
{
public:
    Sprinkler() {}
    Sprinkler(
        std::function<std::optional<Event<d>>()> sprinkleFunc,
        std::function<CausalRelation(const Event<d> &, const Event<d> &)> relationFunc
    )
    :
    sprinkleFunction(sprinkleFunc),
    relationFunction(relationFunc)
    {};

    bool canSprinkle() { return (sprinkleFunction.has_value() && relationFunction.has_value()); }

    Sprinkle<d> sprinkle(int points);

private:
    std::optional<std::function<std::optional<Event<d>>()>> sprinkleFunction = std::nullopt;
    std::optional<std::function<CausalRelation(const Event<d> &, const Event<d> &)>> relationFunction = std::nullopt;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
Sprinkle<d> Sprinkler<d>::sprinkle(const int points)
{
    auto callback = [this](const Event<d> & a, const Event<d> & b)
    {
        if (relationFunction.has_value())
        {
            return relationFunction.value()(a, b);
        }
        std::__throw_runtime_error("Sprinkler built with no way to find relations.");
    };
    Sprinkle<d> sprinkle(callback);
    int generatedPoints = 0;

    while (generatedPoints < points)
    {
        std::optional<Event<d>> potentialPoint = std::nullopt;
        if (sprinkleFunction.has_value())
        {
            potentialPoint = sprinkleFunction.value()();
        }
        if (potentialPoint.has_value())
        {
            generatedPoints++;
            sprinkle.addEvent(*potentialPoint);
        }
        std::cout << "Number of generated points: " << generatedPoints << "\n";
    }

    return sprinkle;
}
