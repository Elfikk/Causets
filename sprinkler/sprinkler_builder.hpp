// Oh boi

#pragma once

#include "sprinkler.hpp"

#include "regions/causal_region.hpp"
#include "regions/cylindrical.hpp"
#include "regions/extended_causal_region.hpp"
#include "regions/rectangular.hpp"
#include "regions/spherical.hpp"

#include "spacetimes/ads.hpp"
#include "spacetimes/minkowski.hpp"

#include "utils/causal_functions.hpp"

#include <functional>
#include <optional>
#include <memory>

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class SprinklerBuilder
{
public:
    SprinklerBuilder() {}

    Minkowski<d> buildMinkowski();
    AdS<d> buildAds(double R0);
    DeSitter<d> buildDeSitter(std::array<double, 2 * d> boundaries);
    // void buildKoluzaKlein(std::array<double, 2*d> bounds);

    CausalRegion<d> buildCausalRegion(const Event<d> &, const Event<d> &);
    CylindricalRegion<d> buildCylinderRegion(
        int axis,
        Event<d> cylinderCentre,
        double cylinderLength,
        double cylinderRadius);
    ExtendedCausalRegion<d> buildExtendedCausalRegion(
        const Event<d-1> & bottom,
        const Event<d-1> & top,
        std::array<double, 2> extension,
        int extension_axis
    );
    ExtendedCausalRegion<d> buildExtendedCausalRegion(
        const Event<d> & bottom,
        const Event<d> & top,
        std::array<double, 2> extension,
        int extension_axis
    );
    RectangularRegion<d> buildRectangularRegion(std::array<double, 2*d> inputBounds);
    SphericalRegion<d> buildSphericalRegion(const Event<d> & sphereCentre, double R);

    RectangularRegion<d> buildRectangularEnclosure(std::array<double, 2*d> inputBounds);

    template<typename SpacetimeT, typename RegionT>
    Sprinkler<d, SpacetimeT, RegionT> getSprinkler(
        SpacetimeT spacetime,
        RegionT region,
        RectangularRegion<d> enclosure)
    {
        return buildSprinkler(spacetime, region, enclosure);
    }

private:
    template<typename SpacetimeT, typename RegionT>
    Sprinkler<d, SpacetimeT, RegionT> buildSprinkler(SpacetimeT, RegionT, RectangularRegion<d>);

    template<typename SpacetimeT>
    CausalRegion<d-1> buildCausalRegion(const Event<d-1> &, const Event<d-1> &, SpacetimeT);

    std::optional<Minkowski<d>> minkowski = std::nullopt;
    std::optional<AdS<d>> ads = std::nullopt;
    std::optional<DeSitter<d>> deSitter = std::nullopt;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
template<typename SpacetimeT, typename RegionT>
Sprinkler<d, SpacetimeT, RegionT> SprinklerBuilder<d>::buildSprinkler(
    SpacetimeT spacetime,
    RegionT region,
    RectangularRegion<d> enclosure)
{
    Sprinkler<d, SpacetimeT, RegionT> sprinkler;
    sprinkler.setSpacetime(spacetime);
    sprinkler.setRegion(region);
    sprinkler.setEnclosingRegion(enclosure);
    return sprinkler;
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
Minkowski<d> SprinklerBuilder<d>::buildMinkowski()
{
    ads = std::nullopt;
    deSitter = std::nullopt;
    minkowski = Minkowski<d>();
    return minkowski.value();
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
AdS<d> SprinklerBuilder<d>::buildAds(double R0)
{
    minkowski = std::nullopt;
    deSitter = std::nullopt;
    ads = AdS<d>(R0);
    return ads.value();
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
DeSitter<d> SprinklerBuilder<d>::buildDeSitter(std::array<double, 2 * d> boundaries)
{
    minkowski = std::nullopt;
    ads = std::nullopt;
    deSitter = DeSitter<d>(boundaries);
    return deSitter.value();
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
CausalRegion<d> SprinklerBuilder<d>::buildCausalRegion(const Event<d> & bottom, const Event<d> & top)
{
    if (minkowski.has_value())
    {
        auto causalFunc = CausalUtils::isInCausalRegion<d>(minkowski.value(), bottom, top);
        auto causalRegion = CausalRegion<d>(causalFunc);
        return causalRegion;
    }
    else if (ads.has_value())
    {
        auto causalFunc = CausalUtils::isInCausalRegion<d>(ads.value(), bottom, top);
        auto causalRegion = CausalRegion<d>(causalFunc);
        return causalRegion;
    }
    else if (deSitter.has_value())
    {
        auto causalFunc = CausalUtils::isInCausalRegion<d>(deSitter.value(), bottom, top);
        auto causalRegion = CausalRegion<d>(causalFunc);
        return causalRegion;
    }
    else
    {
        std::__throw_runtime_error("Tried to build a region without specifying a spacetime first.");
    }
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
template<typename SpacetimeT>
CausalRegion<d-1> SprinklerBuilder<d>::buildCausalRegion(
    const Event<d-1> & bottom,
    const Event<d-1> & top,
    SpacetimeT spacetime)
{
    auto causalFunc = CausalUtils::isInCausalRegion<d-1>(spacetime, bottom, top);
    auto causalRegion = CausalRegion<d-1>(causalFunc);
    return causalRegion;
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
ExtendedCausalRegion<d> SprinklerBuilder<d>::buildExtendedCausalRegion(
    const Event<d> & bottom,
    const Event<d> & top,
    std::array<double, 2> extension,
    int extension_axis
)
{
    std::array<double, d-1> reducedBottom;
    std::array<double, d-1> reducedTop;
    int index = 0;
    for (int i = 0; i < d; i++)
    {
        if (i != extension_axis)
        {
            reducedBottom[index] = bottom[index];
            reducedTop[index] = top[index];
            index += 1;
        }
    }
    Event<d-1> reducedBottomEvent(reducedBottom);
    Event<d-1> reducedTopEvent(reducedTop);

    CausalRegion<d-1> causalRegion([](const Event<d-1> &){ return true; });
    if (ads.has_value())
    {
        if (extension_axis == 1)
        {
            causalRegion = buildCausalRegion(reducedBottomEvent, reducedTopEvent, Minkowski<d-1>());
        }
        else
        {
            causalRegion = buildCausalRegion(reducedBottomEvent, reducedTopEvent, AdS<d-1>(ads->getR0()));
        }
    }
    else if (minkowski.has_value())
    {
        causalRegion = buildCausalRegion(reducedBottomEvent, reducedTopEvent, Minkowski<d-1>());
    }

    return ExtendedCausalRegion<d>(causalRegion, extension, extension_axis);
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
CylindricalRegion<d> SprinklerBuilder<d>::buildCylinderRegion(
    int axis,
    Event<d> cylinderCentre,
    double cylinderLength,
    double cylinderRadius
)
{
    return CylindricalRegion<d>(axis, cylinderCentre, cylinderLength, cylinderRadius);
}

//---------------------------------------------------------------------------------------------------------------------


template<int d>
RectangularRegion<d> SprinklerBuilder<d>::buildRectangularRegion(std::array<double, 2*d> inputBounds)
{
    return RectangularRegion<d>(inputBounds);
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
SphericalRegion<d> SprinklerBuilder<d>::buildSphericalRegion(const Event<d> & sphereCentre, double R)
{
    return SphericalRegion<d>(sphereCentre, R);
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
RectangularRegion<d> SprinklerBuilder<d>::buildRectangularEnclosure(std::array<double, 2*d> inputBounds)
{
    return RectangularRegion<d>(inputBounds);
}
