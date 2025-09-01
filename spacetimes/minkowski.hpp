#pragma once

#include "spacetime.hpp"

#include "event.hpp"

#include <limits>

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class Minkowski: public Spacetime<d>
{
public:
    typedef Minkowski<d> spacetime_class;

    Minkowski();
    Minkowski(std::array<double, 2 * d> boundaries);
    CausalRelation causalRelation(const Event<d> & a, const Event<d> & b) const override;

    double getLowerBound(int i) const { return bounds[2 * i]; }
    double getUpperBound(int i) const { return bounds[2 * i + 1]; }

private:
    double interval(Event<d> & a, Event<d> & b);
    std::array<double, d> metric;
    std::array<double, 2 * d> bounds;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
Minkowski<d>::Minkowski()
{
    metric[0] = -1;
    for (int i = 1; i < d; i++)
    {
        metric[i] = 1;
    }

    for (int i = 0; i < d; i++)
    {
        bounds[2 * i] = std::numeric_limits<double>::lowest();
        bounds[2 * i + 1] = std::numeric_limits<double>::max();
    }
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
Minkowski<d>::Minkowski(std::array<double, 2 * d> boundaries) : bounds(boundaries)
{
    metric[0] = -1;
    for (int i = 1; i < d; i++)
    {
        metric[i] = 1;
    }
}

//---------------------------------------------------------------------------------------------------------------------

// This is specific to Cartesian style coordinates - could want spherical...
template<int d>
CausalRelation Minkowski<d>::causalRelation(const Event<d> & a, const Event<d> & b) const
{
    double interval = 0;
    for (int i = 0; i < d; i++)
    {
        interval += metric[i] * (b[i] - a[i]) * (b[i] - a[i]);
    }

    if (interval > 0)
    {
        return CausalRelation::Spacelike;
    }
    else if (interval == 0)
    {
        return CausalRelation::TimeLike;
    }
    return a[0] < b[0] ? CausalRelation::CausalFuture : CausalRelation::CausalPast;
}
