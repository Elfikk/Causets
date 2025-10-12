#pragma once

#include "region.hpp"

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class SphericalRegion: public Region<d>
{
public:
    SphericalRegion(Event<d> sphereCentre, long double R) : centre(sphereCentre), radius(R) {}
    bool isInside(const Event<d> &) const override;

private:
    Event<d> centre;
    long double radius;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
bool SphericalRegion<d>::isInside(const Event<d> & event) const
{
    long double sum = 0;
    for (int i = 0; i < d; i++)
    {
        sum += (centre[i] - event[i]) * (centre[i] - event[i]);
    }
    return sum < radius;
}
