#pragma once

#include "regions/rectangular.hpp"

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class PeriodicRectangular: public Region<d>
{
public:
    PeriodicRectangular(std::array<long double, 2*d> inputBounds);

    bool isInside(const Event<d> &) const override;
    long double getLowerBound(int coordinate) const { return rectangularRegion.getLowerBound(coordinate); };
    long double getUpperBound(int coordinate) const { return rectangularRegion.getUpperBound(coordinate); };

    long double getMaxLength() const { return maxLength; }

private:
    RectangularRegion<d> rectangularRegion;
    long double maxLength = 0;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
PeriodicRectangular<d>::PeriodicRectangular(std::array<long double, 2*d> inputBounds)
:
rectangularRegion(RectangularRegion<d>(inputBounds))
{
    for (int i = 0; i < d; i++)
    {
        maxLength = std::max(maxLength, getUpperBound(i) - getLowerBound(i));
    }
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
bool PeriodicRectangular<d>::isInside(const Event<d> & event) const
{
    const auto time = event.time();
    const auto lower = getLowerBound(0);
    const auto upper = getUpperBound(0);
    return (lower <= time) && (time <= upper);
}
