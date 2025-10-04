// Oh boi

#pragma once

#include "sprinkler.hpp"

#include "regions/causal_region.hpp"
#include "regions/cylindrical.hpp"
#include "regions/rectangular.hpp"

#include "spacetimes/ads.hpp"
#include "spacetimes/minkowski.hpp"

#include "utils/causal_functions.hpp"

#include <functional>
#include <memory>

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class SprinklerBuilder
{
public:
    SprinklerBuilder() {}

    void buildMinkowski();
    void buildAds(double R0);
    // void buildKoluzaKlein(std::array<double, 2*d> bounds);

    void buildCausalRegion(const Event<d> &, const Event<d> &);
    void buildCylinderRegion(int axis, Event<d> cylinderCentre, double cylinderLength, double cylinderRadius);
    void buildRectangularRegion(std::array<double, 2*d> inputBounds);

    void buildRectangularEnclosure(std::array<double, 2*d> inputBounds);

    Sprinkler<d> getSprinkler() { return sprinkler; }

private:
    Sprinkler<d> sprinkler;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
void SprinklerBuilder<d>::buildMinkowski()
{
    auto minkowski = std::make_unique<Minkowski<d>>();
    sprinkler.setSpacetime(std::move(minkowski));
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
void SprinklerBuilder<d>::buildAds(double R0)
{
    auto ads = std::make_unique<AdS<d>>(R0);
    sprinkler.setSpacetime(std::move(ads));
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
void SprinklerBuilder<d>::buildCausalRegion(const Event<d> & bottom, const Event<d> & top)
{
    auto spacetime = sprinkler.getSpacetime();
    auto causalFunc = CausalUtils::isInCausalRegion<d>(spacetime, bottom, top);
    auto causalRegion = std::make_unique<CausalRegion<d>>(causalFunc);
    sprinkler.setRegion(std::move(causalRegion));
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
void SprinklerBuilder<d>::buildRectangularRegion(std::array<double, 2*d> inputBounds)
{
    auto rectangularRegion = std::make_unique<RectangularRegion<d>>(inputBounds);
    sprinkler.setRegion(std::move(rectangularRegion));
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
void SprinklerBuilder<d>::buildRectangularEnclosure(std::array<double, 2*d> inputBounds)
{
    auto rectangularRegion = std::make_unique<RectangularRegion<d>>(inputBounds);
    sprinkler.setEnclosingRegion(std::move(rectangularRegion));
}
