#pragma once

#include "region.hpp"

#include "event.hpp"

#include "spacetimes/spacetime.hpp"

#include <array>
#include <functional>
#include <iterator>

//---------------------------------------------------------------------------------------------------------------------

// Causal Region - Overlap between the causal future and causal past of two
// different points. Two hypercones in Minkowski or any conformal (?) spacetime
// like AdS.
template<int d>
class CausalRegion: public Region<d>
{
public:
    CausalRegion(std::function<bool(const Event<d> &)> func) : causalFunction(func) {}
    ~CausalRegion() {};

    // I want to be able to switch the strategy of this check, depending on the
    // spacetime
    bool isInside(const Event<d> &) const override;

private:
    // Default function should check causal relation with top and bottom events
    // of the causal region; if more clever way exists then great i.e from the
    // geometrical cone definition.
    std::function<bool(const Event<d> &)> causalFunction;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
bool CausalRegion<d>::isInside(const Event<d> & event) const
{
    return causalFunction(event);
}
