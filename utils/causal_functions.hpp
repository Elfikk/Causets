#pragma once

#include "event.hpp"

#include "spacetimes/spacetime.hpp"

#include <functional>

//---------------------------------------------------------------------------------------------------------------------

namespace CausalUtils
{

template<int d, typename SpacetimeT>
std::function<bool(Event<d>&)> isInCausalRegion(SpacetimeT * spacetime, Event<d> bottom, Event<d> top)
{
    // I don't think this will work; copies of these parameters surely will leave the scope of this builder function and be deleted?
    auto causalFunction = [spacetime, bottom, top](Event<d> & event)
    {
        const auto bottomRelation = spacetime->causalRelation(bottom, event);
        const auto topRelation = spacetime->causalRelation(event, top);
        return ((bottomRelation == CausalRelation::CausalFuture) && (topRelation == CausalRelation::CausalPast));
    };
    return causalFunction;
}

// Partial specialisations are a bit nasty really - there probably is a better way
template<int d>
std::function<bool(Event<d>&)> isInCausalRegion(AdS<d> * spacetime, Event<d> bottom, Event<d> top)
{
    return isInCausalRegion<d, AdS<d>>(spacetime, bottom, top);
}

template<int d>
std::function<bool(Event<d>&)> isInCausalRegion(Minkowski<d> * spacetime, Event<d> bottom, Event<d> top)
{
    return isInCausalRegion<d, Minkowski<d>>(spacetime, bottom, top);
}

};
