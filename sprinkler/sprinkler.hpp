#pragma once

#include "regions/rectangular.hpp"
#include "regions/region.hpp"

#include "spacetimes/spacetime.hpp"

#include "sprinkler/sprinkle.hpp"
#include "sprinkler/sprinkle_strategies.hpp"

#include <memory>

template<int d>
class Sprinkle;

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class Sprinkler
{
public:
    Sprinkler() {}
    Sprinkler(Sprinkler<d> &); // How does this deal with unique pointers? TL;DR it didn't

    bool canSprinkle() { return spacetime && region && enclosingRegion; }

    Sprinkle<d> sprinkle(int points);

    Spacetime<d> * getSpacetime() { return spacetime.get(); }

    void setSpacetime(std::unique_ptr<Spacetime<d>> spacetime);
    void setRegion(std::unique_ptr<Region<d>> region);
    void setEnclosingRegion(std::unique_ptr<RectangularRegion<d>> newRegion) { enclosingRegion = std::move(newRegion); }

private:
    std::unique_ptr<Spacetime<d>> spacetime = nullptr;
    std::unique_ptr<Region<d>> region = nullptr;
    std::unique_ptr<RectangularRegion<d>> enclosingRegion = nullptr;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
void Sprinkler<d>::setSpacetime(std::unique_ptr<Spacetime<d>> newSpacetime)
{
    spacetime = std::move(newSpacetime);
    region = nullptr;
    enclosingRegion = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
void Sprinkler<d>::setRegion(std::unique_ptr<Region<d>> newRegion)
{
    region = std::move(newRegion);
    enclosingRegion = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
Sprinkler<d>::Sprinkler(Sprinkler<d> & other)
{
    setSpacetime(std::move(other.spacetime));
    setRegion(std::move(other.region));
    setEnclosingRegion(std::move(other.enclosingRegion));
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
Sprinkle<d> Sprinkler<d>::sprinkle(int points)
{
    Sprinkle<d> sprinkle;
    int generatedPoints = 0;

    while (generatedPoints < points)
    {
        auto potentialPoint = SprinkleStrategy::minkowskiRegionSprinkleEvent(*region, *enclosingRegion);
        if (!potentialPoint)
        {
            generatedPoints++;
            sprinkle.addEvent(*potentialPoint);
        }
    }

    return sprinkle;
}
