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

template<int d, typename SpacetimeT, typename RegionT>
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

    bool canSprinkle() { return (sprinkleFunction.has_value() && relationFunction.has_value()) || (spacetime.has_value() && region.has_value() && enclosingRegion.has_value()); }

    Sprinkle<d> sprinkle(int points);

    void setSpacetime(SpacetimeT newSpacetime) { spacetime = newSpacetime; };
    void setRegion(RegionT newRegion) { region = newRegion; }
    void setEnclosingRegion(RectangularRegion<d> newRegion) { enclosingRegion = newRegion; }

private:
    std::optional<SpacetimeT> spacetime = std::nullopt;
    std::optional<RegionT> region = std::nullopt;
    std::optional<RectangularRegion<d>> enclosingRegion = std::nullopt;

    std::optional<std::function<std::optional<Event<d>>()>> sprinkleFunction = std::nullopt;
    std::optional<std::function<CausalRelation(const Event<d> &, const Event<d> &)>> relationFunction = std::nullopt;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d, typename SpacetimeT, typename RegionT>
Sprinkle<d> Sprinkler<d, SpacetimeT, RegionT>::sprinkle(const int points)
{
    auto callback = [this](const Event<d> & a, const Event<d> & b)
    {
        if (spacetime.has_value())
        {
            return spacetime->causalRelation(a,b);
        }
        else if (relationFunction.has_value())
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
        else
        {
            potentialPoint = SprinkleStrategy::sprinkleEvent<d, SpacetimeT>(&region.value(), enclosingRegion.value());
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
