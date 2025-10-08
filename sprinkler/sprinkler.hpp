#pragma once

#include "regions/rectangular.hpp"
#include "regions/region.hpp"

#include "spacetimes/spacetime.hpp"

#include "sprinkler/sprinkle.hpp"
#include "sprinkler/sprinkle_strategies.hpp"

#include <optional>
#include <memory>

template<int d, typename SpacetimeT>
class Sprinkle;

//---------------------------------------------------------------------------------------------------------------------

template<int d, typename SpacetimeT, typename RegionT>
class Sprinkler
{
public:
    Sprinkler() {}

    bool canSprinkle() { return spacetime.has_value() && region.has_value() && enclosingRegion.has_value(); }

    Sprinkle<d, SpacetimeT> sprinkle(int points);

    void setSpacetime(SpacetimeT newSpacetime) { spacetime = newSpacetime; };
    void setRegion(RegionT newRegion) { region = newRegion; }
    void setEnclosingRegion(RectangularRegion<d> newRegion) { enclosingRegion = newRegion; }

private:
    std::optional<SpacetimeT> spacetime = std::nullopt;
    std::optional<RegionT> region = std::nullopt;
    std::optional<RectangularRegion<d>> enclosingRegion = std::nullopt;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d, typename SpacetimeT, typename RegionT>
Sprinkle<d, SpacetimeT> Sprinkler<d, SpacetimeT, RegionT>::sprinkle(const int points)
{
    Sprinkle<d, SpacetimeT> sprinkle(spacetime.value());
    int generatedPoints = 0;

    while (generatedPoints < points)
    {
        auto potentialPoint = SprinkleStrategy::sprinkleEvent<d, SpacetimeT>(region.value(), enclosingRegion.value());
        if (potentialPoint.has_value())
        {
            generatedPoints++;
            sprinkle.addEvent(*potentialPoint);
        }
    }

    return sprinkle;
}
