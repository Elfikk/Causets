#pragma once

#include "regions/rectangular.hpp"
#include "regions/region.hpp"

#include "spacetimes/spacetime.hpp"

#include <memory>

template<int d>
class Sprinkle;

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class Sprinkler
{
public:
    Sprinkler() {}

    bool canSprinkle() { return !spacetime && !region && !enclosingRegion; }

    std::unique_ptr<Sprinkle<d>> sprinkle(int points);

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
