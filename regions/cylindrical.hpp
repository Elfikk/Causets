#pragma once

#include "region.hpp"

#include "event.hpp"

#include <array>

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class CylindricalRegion : public Region<d>
{
public:
    CylindricalRegion(
        int axis,
        Event<d> cylinderCentre,
        long double cylinderLength,
        long double cylinderRadius
    );
    bool isInside(const Event<d> &) const override;

private:
    int axisCoordinate;
    std::array<long double, 2> axisBounds;
    Event<d> centre;
    long double radius;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
CylindricalRegion<d>::CylindricalRegion(
    int axis,
    Event<d> cylinderCentre,
    long double cylinderLength,
    long double cylinderRadius
)
:
axisCoordinate(axis),
centre(cylinderCentre),
radius(cylinderRadius)
{
    const auto halfLength = cylinderLength/2;
    axisBounds = std::array<long double, 2>({cylinderCentre[axis] - halfLength, cylinderCentre[axis] + halfLength});
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
bool CylindricalRegion<d>::isInside(const Event<d> & event) const
{
    // Check within length of cylinder
    auto axisCoordValue = event[axisCoordinate];
    if ((axisBounds[0] > axisCoordValue) || (axisBounds[1] < axisCoordValue))
    {
        return false;
    }

    long double sum_sq = 0;
    // Now check all other coordinates radius smaller than the advertised one
    for (int i = 0; i < d; i++)
    {
        if (i != axisCoordinate)
        {
            sum_sq += (event[i] - centre[i]) * (event[i] - centre[i]);
        }
    }

    return sum_sq < radius * radius;
}
