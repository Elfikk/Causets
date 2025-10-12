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

};
