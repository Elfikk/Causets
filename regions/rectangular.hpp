#pragma once

#include "region.hpp"

#include "event.hpp"

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class RectangularRegion : public Region<d>
{
public:
    RectangularRegion(std::array<double, 2*d> inputBounds) : bounds(inputBounds) {}
    bool isInside(Event<d> &) override;
    double getLowerBound(int coordinate) { return bounds[2 * coordinate]; };
    double getUpperBound(int coordinate) { return bounds[2 * coordinate + 1]; };

private:
    std::array<double, 2*d> bounds;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
bool RectangularRegion<d>::isInside(Event<d> & event)
{
    for (int i = 0; i < d; i++)
    {
        const auto lower = bounds[2 * i];
        const auto upper = bounds[2 * i + 1];
        if ((lower > event[i]) || (event[i] > upper))
        {
            return false;
        }
    }
    return true;
}
