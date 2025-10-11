#pragma once

#include "event.hpp"

#include "spacetimes/spacetime.hpp"

#include <functional>

//---------------------------------------------------------------------------------------------------------------------

namespace CausalUtils
{

template<int d, typename SpacetimeT>
std::function<bool(const Event<d>&)> isInCausalRegion(const SpacetimeT & spacetime, Event<d> bottom, Event<d> top)
{
    auto causalFunction = [spacetime, bottom, top](const Event<d> & event)
    {
        const auto bottomRelation = spacetime.causalRelation(bottom, event);
        const auto topRelation = spacetime.causalRelation(event, top);
        return ((bottomRelation == CausalRelation::CausalFuture) && (topRelation == CausalRelation::CausalFuture));
    };
    return causalFunction;
}

//---------------------------------------------------------------------------------------------------------------------

// Partial specialisations are a bit nasty really - there probably is a better way
template<int d>
std::function<bool(const Event<d>&)> isInCausalRegion(const AdS<d> spacetime, Event<d> bottom, Event<d> top)
{
    return isInCausalRegion<d, AdS<d>>(spacetime, bottom, top);
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
std::function<bool(const Event<d>&)> isInCausalRegion(const Minkowski<d> spacetime, Event<d> bottom, Event<d> top)
{
    return isInCausalRegion<d, Minkowski<d>>(spacetime, bottom, top);
}

};
