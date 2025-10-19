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
#include <variant>

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class SprinklerBuilder
{
public:
    SprinklerBuilder() {}

    Minkowski<d> buildMinkowski();
    AdS<d> buildAds(long double R0);
    DeSitter<d> buildDeSitter(std::array<long double, 2 * d> boundaries);
    // void buildKoluzaKlein(std::array<long double, 2*d> bounds);

    CausalRegion<d> buildCausalRegion(const Event<d> &, const Event<d> &);
    CylindricalRegion<d> buildCylinderRegion(
        int axis,
        Event<d> cylinderCentre,
        long double cylinderLength,
        long double cylinderRadius);
    ExtendedCausalRegion<d> buildExtendedCausalRegion(
        const Event<d-1> & bottom,
        const Event<d-1> & top,
        std::array<long double, 2> extension,
        int extension_axis
    );
    ExtendedCausalRegion<d> buildExtendedCausalRegion(
        const Event<d> & bottom,
        const Event<d> & top,
        std::array<long double, 2> extension,
        int extension_axis
    );
    RectangularRegion<d> buildRectangularRegion(std::array<long double, 2*d> inputBounds);
    SphericalRegion<d> buildSphericalRegion(const Event<d> & sphereCentre, long double R);

    RectangularRegion<d> buildRectangularEnclosure(std::array<long double, 2*d> inputBounds);

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

    enum class ActiveSpacetime
    {
        AdS,
        DeSitter,
        Minkowski,
        None
    };
    ActiveSpacetime currentSpacetime = ActiveSpacetime::None;
    // Is there a different way to make the variant not complain about not being initialised?
    std::variant<AdS<d>, DeSitter<d>, Minkowski<d>> spacetime = Minkowski<d>();

    enum class ActiveRegion
    {
        Causal,
        Cylindrical,
        ExtendedCausal,
        Rectangular,
        Spherical,
        None
    };
    ActiveRegion currentRegion = ActiveRegion::None;

    enum class ActiveEncloser
    {
        Rectangular,
        None
    };
    ActiveEncloser currentEncloser = ActiveEncloser::None;
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
    spacetime = Minkowski<d>();
    currentSpacetime = ActiveSpacetime::Minkowski;
    currentRegion = ActiveRegion::None;
    currentEncloser = ActiveEncloser::None;
    return std::get<Minkowski<d>>(spacetime);
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
AdS<d> SprinklerBuilder<d>::buildAds(long double R0)
{
    spacetime = AdS<d>(R0);
    currentSpacetime = ActiveSpacetime::AdS;
    currentRegion = ActiveRegion::None;
    currentEncloser = ActiveEncloser::None;
    return std::get<AdS<d>>(spacetime);
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
DeSitter<d> SprinklerBuilder<d>::buildDeSitter(std::array<long double, 2 * d> boundaries)
{
    spacetime = DeSitter<d>(boundaries);
    currentSpacetime = ActiveSpacetime::DeSitter;
    currentRegion = ActiveRegion::None;
    currentEncloser = ActiveEncloser::None;
    return std::get<DeSitter<d>>(spacetime);
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
CausalRegion<d> SprinklerBuilder<d>::buildCausalRegion(const Event<d> & bottom, const Event<d> & top)
{
    if (currentSpacetime != ActiveSpacetime::None)
    {
        currentRegion = ActiveRegion::Causal;
        currentEncloser = ActiveEncloser::None;
    }
    if (currentSpacetime == ActiveSpacetime::Minkowski)
    {
        auto causalFunc = CausalUtils::isInCausalRegion<d>(std::get<Minkowski<d>>(spacetime), bottom, top);
        auto causalRegion = CausalRegion<d>(causalFunc);
        return causalRegion;
    }
    else if (currentSpacetime == ActiveSpacetime::AdS)
    {
        auto causalFunc = CausalUtils::isInCausalRegion<d>(std::get<AdS<d>>(spacetime), bottom, top);
        auto causalRegion = CausalRegion<d>(causalFunc);
        return causalRegion;
    }
    else if (currentSpacetime == ActiveSpacetime::DeSitter)
    {
        auto causalFunc = CausalUtils::isInCausalRegion<d>(std::get<DeSitter<d>>(spacetime), bottom, top);
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
    std::array<long double, 2> extension,
    int extension_axis
)
{
    std::array<long double, d-1> reducedBottom;
    std::array<long double, d-1> reducedTop;
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

    if ((currentSpacetime != ActiveSpacetime::None) && (currentSpacetime != ActiveSpacetime::DeSitter))
    {
        currentRegion = ActiveRegion::ExtendedCausal;
        currentEncloser = ActiveEncloser::None;
    }
    if (currentSpacetime == ActiveSpacetime::AdS)
    {
        if (extension_axis == 1)
        {
            causalRegion = buildCausalRegion(reducedBottomEvent, reducedTopEvent, Minkowski<d-1>());
        }
        else
        {
            causalRegion = buildCausalRegion(reducedBottomEvent, reducedTopEvent, AdS<d-1>(std::get<AdS<d>>(spacetime).getR0()));
        }
    }
    else if (currentSpacetime == ActiveSpacetime::Minkowski)
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
    long double cylinderLength,
    long double cylinderRadius
)
{
    currentRegion = ActiveRegion::Cylindrical;
    currentEncloser = ActiveEncloser::None;
    return CylindricalRegion<d>(axis, cylinderCentre, cylinderLength, cylinderRadius);
}

//---------------------------------------------------------------------------------------------------------------------


template<int d>
RectangularRegion<d> SprinklerBuilder<d>::buildRectangularRegion(std::array<long double, 2*d> inputBounds)
{
    currentRegion = ActiveRegion::Rectangular;
    currentEncloser = ActiveEncloser::None;
    return RectangularRegion<d>(inputBounds);
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
SphericalRegion<d> SprinklerBuilder<d>::buildSphericalRegion(const Event<d> & sphereCentre, long double R)
{
    currentRegion = ActiveRegion::Spherical;
    currentEncloser = ActiveEncloser::None;
    return SphericalRegion<d>(sphereCentre, R);
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
RectangularRegion<d> SprinklerBuilder<d>::buildRectangularEnclosure(std::array<long double, 2*d> inputBounds)
{
    currentEncloser = ActiveEncloser::Rectangular;
    return RectangularRegion<d>(inputBounds);
}
