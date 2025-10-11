#pragma once

#include "regions/causal_region.hpp"
#include "regions/region.hpp"

#include <array>

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class ExtendedCausalRegion: public Region<d>
{
public:
    ExtendedCausalRegion(
        CausalRegion<d-1> region,
        std::array<double, 2> extension,
        int extension_axis
    )
    :
    causalRegion(region),
    bounds(extension),
    axis(extension_axis) {}
    ~ExtendedCausalRegion() {};

    bool isInside(const Event<d> &) const override;

private:
    CausalRegion<d-1> causalRegion;
    std::array<double, 2> bounds;
    int axis;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
bool ExtendedCausalRegion<d>::isInside(const Event<d> & event) const
{
    if ((event[axis] < bounds[0]) || (event[axis] > bounds[1]))
    {
        return false;
    }

    std::array<double, d-1> reduced;
    int index = 0;
    for (int i = 0; i < d; i++)
    {
        reduced[i] = event[i];
        if (i != axis)
        {
            reduced[index] = event[index];
            index += 1;
        }
    }
    Event<d-1> reducedEvent(reduced);
    return causalRegion.isInside(reducedEvent);
}
