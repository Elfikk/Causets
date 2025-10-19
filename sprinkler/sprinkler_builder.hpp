// Oh boi

#pragma once

#include "sprinkler.hpp"

#include "regions/causal_region.hpp"
#include "regions/cylindrical.hpp"
#include "regions/extended_causal_region.hpp"
#include "regions/rectangular.hpp"
#include "regions/spherical.hpp"

#include "spacetimes/ads.hpp"
#include "spacetimes/de_sitter.hpp"
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

    template<typename SpacetimeT, typename RegionT>
    Sprinkler<d, SpacetimeT, RegionT> getSprinkler();

private:
    template<typename SpacetimeT, typename RegionT>
    Sprinkler<d, SpacetimeT, RegionT> buildSprinkler(SpacetimeT, RegionT, RectangularRegion<d>);

    std::function<std::optional<Event<d>>(Region<d> *, RectangularRegion<d> &)> selectSpacetimeSprinkleFunc();
    std::function<std::optional<Event<d>>()> selectSprinklerSprinkleFunc(std::function<std::optional<Event<d>>(Region<d> *,RectangularRegion<d> &)>);
    std::function<CausalRelation(const Event<d> &, const Event<d> &)> selectCausalFunction();

    template<typename RegionT>
    std::function<std::optional<Event<d>>()> buildSprinkleFunction(std::function<std::optional<Event<d>>(Region<d> *,RectangularRegion<d> &)>);

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
    std::variant<
        CausalRegion<d>,
        CylindricalRegion<d>,
        ExtendedCausalRegion<d>,
        RectangularRegion<d>,
        SphericalRegion<d>
    > region = RectangularRegion<d>({});

    enum class ActiveEncloser
    {
        Rectangular,
        None
    };
    ActiveEncloser currentEncloser = ActiveEncloser::None;
    std::variant<RectangularRegion<d>> enclosure = RectangularRegion<d>({});
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
    std::function<bool(const Event<d>&)> causalFunc = [](const Event<d>&){ return true; };
    switch (currentSpacetime)
    {
        case ActiveSpacetime::Minkowski:
        {
            causalFunc = CausalUtils::isInCausalRegion<d>(std::get<Minkowski<d>>(spacetime), bottom, top);
            break;
        }
        case ActiveSpacetime::AdS:
        {
            causalFunc = CausalUtils::isInCausalRegion<d>(std::get<AdS<d>>(spacetime), bottom, top);
            break;
        }
        case ActiveSpacetime::DeSitter:
        {
            causalFunc = CausalUtils::isInCausalRegion<d>(std::get<DeSitter<d>>(spacetime), bottom, top);
            break;
        }
        case ActiveSpacetime::None:
        {
            std::__throw_runtime_error("Tried to build a region without specifying a spacetime first.");
        }
    }
    region = CausalRegion<d>(causalFunc);
    currentRegion = ActiveRegion::Causal;
    currentEncloser = ActiveEncloser::None;
    return std::get<CausalRegion<d>>(region);
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
    switch (currentSpacetime)
    {
        case ActiveSpacetime::AdS:
        {
            if (extension_axis == 1)
            {
                causalRegion = buildCausalRegion(reducedBottomEvent, reducedTopEvent, Minkowski<d-1>());
            }
            else
            {
                causalRegion = buildCausalRegion(reducedBottomEvent, reducedTopEvent, AdS<d-1>(std::get<AdS<d>>(spacetime).getR0()));
            }
            break;
        }
        case ActiveSpacetime::Minkowski:
        {
            causalRegion = buildCausalRegion(reducedBottomEvent, reducedTopEvent, Minkowski<d-1>());
            break;
        }
        case ActiveSpacetime::DeSitter:
        {
            std::__throw_runtime_error("De Sitter does not have an ECR implementation.");
            break;
        }
        case ActiveSpacetime::None:
        {
            std::__throw_runtime_error("Need a spacetime to build a region.");
        }
    }
    currentRegion = ActiveRegion::ExtendedCausal;
    currentEncloser = ActiveEncloser::None;
    region = ExtendedCausalRegion<d>(causalRegion, extension, extension_axis);
    return std::get<ExtendedCausalRegion<d>>(region);
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
    if (currentSpacetime == ActiveSpacetime::None)
    {
        std::__throw_runtime_error("Need a spacetime to build a region.");
    }
    currentRegion = ActiveRegion::Cylindrical;
    currentEncloser = ActiveEncloser::None;
    region = CylindricalRegion<d>(axis, cylinderCentre, cylinderLength, cylinderRadius);
    return std::get<CylindricalRegion<d>>(region);
}

//---------------------------------------------------------------------------------------------------------------------


template<int d>
RectangularRegion<d> SprinklerBuilder<d>::buildRectangularRegion(std::array<long double, 2*d> inputBounds)
{
    if (currentSpacetime == ActiveSpacetime::None)
    {
        std::__throw_runtime_error("Need a spacetime to build a region.");
    }
    currentRegion = ActiveRegion::Rectangular;
    currentEncloser = ActiveEncloser::None;
    region = RectangularRegion<d>(inputBounds);
    return std::get<RectangularRegion<d>>(region);
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
SphericalRegion<d> SprinklerBuilder<d>::buildSphericalRegion(const Event<d> & sphereCentre, long double R)
{
    if (currentSpacetime == ActiveSpacetime::None)
    {
        std::__throw_runtime_error("Need a spacetime to build a region.");
    }
    currentRegion = ActiveRegion::Spherical;
    currentEncloser = ActiveEncloser::None;
    region = SphericalRegion<d>(sphereCentre, R);
    return std::get<SphericalRegion<d>>(region);
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
RectangularRegion<d> SprinklerBuilder<d>::buildRectangularEnclosure(std::array<long double, 2*d> inputBounds)
{
    if ((currentSpacetime == ActiveSpacetime::None) || (currentRegion == ActiveRegion::None))
    {
        std::__throw_runtime_error("Need a spacetime and region before building an enclosing region.");
    }
    currentEncloser = ActiveEncloser::Rectangular;
    enclosure = RectangularRegion<d>(inputBounds);
    return std::get<RectangularRegion<d>>(enclosure);
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
template<typename RegionT>
std::function<std::optional<Event<d>>()> SprinklerBuilder<d>::buildSprinkleFunction(
    std::function<std::optional<Event<d>>(Region<d> *,RectangularRegion<d> &)> func
)
{
    return [this, func](){ return func(&std::get<RegionT>(region), std::get<RectangularRegion<d>>(enclosure)); };
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
std::function<std::optional<Event<d>>(Region<d> *,RectangularRegion<d> &)> SprinklerBuilder<d>::selectSpacetimeSprinkleFunc()
{
    switch (currentSpacetime)
    {
        case ActiveSpacetime::Minkowski:
        {
            return SprinkleStrategy::minkowskiEventSprinkle<d>;
        }
        case ActiveSpacetime::AdS:
        {
            return SprinkleStrategy::adsEventSprinkle<d>;
        }
        case ActiveSpacetime::DeSitter:
        {
            if (d != 2)
            {
                std::__throw_runtime_error("De Sitter Only Implemented for d=2.");
            }
            return SprinkleStrategy::deSitterEventSprinkle;
        }
        case ActiveSpacetime::None:
        {
            std::__throw_runtime_error("Uninitialised spacetime whilst selecting sprinkle function.");
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
std::function<std::optional<Event<d>>()> SprinklerBuilder<d>::selectSprinklerSprinkleFunc(
    std::function<std::optional<Event<d>>(Region<d> *,RectangularRegion<d> &)> fullSprinkleFunc
)
{
    switch (currentRegion)
    {
        case ActiveRegion::Causal:
        {
            return buildSprinkleFunction<CausalRegion<d>>(fullSprinkleFunc);
        }
        case ActiveRegion::Cylindrical:
        {
            return buildSprinkleFunction<CylindricalRegion<d>>(fullSprinkleFunc);
        }
        case ActiveRegion::ExtendedCausal:
        {
            return buildSprinkleFunction<ExtendedCausalRegion<d>>(fullSprinkleFunc);
        }
        case ActiveRegion::Rectangular:
        {
            return buildSprinkleFunction<RectangularRegion<d>>(fullSprinkleFunc);
        }
        case ActiveRegion::Spherical:
        {
            return buildSprinkleFunction<SphericalRegion<d>>(fullSprinkleFunc);
        }
        case ActiveRegion::None:
        {
            std::__throw_runtime_error("Unitialised region when making reduced sprinkle function.");
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
std::function<CausalRelation(const Event<d> &, const Event<d> &)> SprinklerBuilder<d>::selectCausalFunction()
{
    switch (currentSpacetime)
    {
        case ActiveSpacetime::Minkowski:
        {
            auto minkowski = std::get<Minkowski<d>>(spacetime);
            return [minkowski](const Event<d> & a, const Event<d> & b) { return minkowski.causalRelation(a, b); };
        }
        case ActiveSpacetime::AdS:
        {
            auto ads = std::get<AdS<d>>(spacetime);
            return [ads](const Event<d> & a, const Event<d> & b) { return ads.causalRelation(a, b); };
        }
        case ActiveSpacetime::DeSitter:
        {
            auto deSitter = std::get<DeSitter<d>>(spacetime);
            return [deSitter](const Event<d> & a, const Event<d> & b) { return deSitter.causalRelation(a, b); };
        }
        case ActiveSpacetime::None:
        {
            std::__throw_runtime_error("Uninitialised spacetime whilst selecting causal function.");
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
template<typename SpacetimeT, typename RegionT>
Sprinkler<d, SpacetimeT, RegionT> SprinklerBuilder<d>::getSprinkler()
{
    if (currentSpacetime == ActiveSpacetime::None || currentRegion == ActiveRegion::None || currentEncloser == ActiveEncloser::None)
    {
        // This is not quite true - the encloser is only necessary as for now we
        // always rely on sprinkling into a rectangular region.
        // Should have really called it a hypercubic region shouldn't have I?
        std::__throw_runtime_error("Cannot sprinkle without a spacetime, region and enclosing region not defined.");
    }
    auto fullSprinkleFunc = selectSpacetimeSprinkleFunc();
    auto sprinkleFunc = selectSprinklerSprinkleFunc(fullSprinkleFunc);
    auto causalFunc = selectCausalFunction();
    return Sprinkler<d, SpacetimeT, RegionT>(sprinkleFunc, causalFunc);
}
