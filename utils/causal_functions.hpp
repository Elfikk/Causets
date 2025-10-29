#pragma once

#include "event.hpp"

#include "regions/periodic_rectangular.hpp"

#include "spacetimes/spacetime.hpp"

#include <cmath>
#include <functional>
#include <limits>

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

template<int d>
std::function<CausalRelation(const Event<d> &, const Event<d> &)> periodicRectangularRelations(
    const PeriodicRectangular<d> & region
)
{
    const auto causalRelations = [region](const Event<d> & a, const Event<d> & b)
    {
        const auto deltaTime = b[0] - a[0];
        const auto critTime = region.getMaxLength();
        if (deltaTime >= critTime)
        {
            return CausalRelation::CausalFuture;
        }
        else if (deltaTime <= -critTime)
        {
            return CausalRelation::CausalPast;
        }
        else
        {
            // Do the usual separation calculation in conformal spacetimes, but with periodic bobs.
            long double spaceSeparation = 0;
            for (int i = 1; i < d; i++)
            {
                const auto period = region.getUpperBound(i) - region.getLowerBound(i);
                long double separation = std::numeric_limits<long double>::max();
                for (int j = -1; j < 2; j++)
                {
                    const auto pseudoOrdinate = j * period + b[i];
                    separation = std::min(separation, (pseudoOrdinate - a[i]) * (pseudoOrdinate - a[i]));
                }
                spaceSeparation += separation;
            }

            const auto timeSeparation = deltaTime * deltaTime;
            if (timeSeparation == spaceSeparation)
            {
                return CausalRelation::TimeLike;
            }
            else if (timeSeparation > spaceSeparation)
            {
                if (deltaTime > 0)
                {
                    return CausalRelation::CausalFuture;
                }
                else if (deltaTime < 0)
                {
                    return CausalRelation::CausalPast;
                }
            }
        }
        return CausalRelation::Spacelike;
    };
    return causalRelations;
}

};
