#pragma once

#include "spacetime.hpp"

#include "event.hpp"

#include <iterator>
#include <limits>

//---------------------------------------------------------------------------------------------------------------------

struct minkowski_spacetime: public generic_spacetime {};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class Minkowski: public Spacetime<d>
{
public:
    typedef minkowski_spacetime spacetime_class;

    Minkowski();
    Minkowski(std::array<double, 2 * d> boundaries);
    CausalRelation causalRelation(const Event<d> & a, const Event<d> & b) const override;

    double getLowerBound(int i) const { return bounds[2 * i]; }
    double getUpperBound(int i) const { return bounds[2 * i + 1]; }

private:
    double interval(const Event<d> & a, const Event<d> & b) const;
    std::array<double, d> metric;
    std::array<double, 2 * d> bounds;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
struct spacetime_traits<Minkowski<d>>
{
    typedef typename Minkowski<d>::spacetime_class spacetime_class;
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
    const auto separation = interval(a, b);
    if (separation > 0)
    {
        return CausalRelation::Spacelike;
    }
    else if (separation == 0)
    {
        return CausalRelation::TimeLike;
    }
    return a[0] < b[0] ? CausalRelation::CausalFuture : CausalRelation::CausalPast;
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
double Minkowski<d>::interval(const Event<d> & a, const Event<d> & b) const
{
    double separation = 0;
    for (int i = 0; i < d; i++)
    {
        separation += metric[i] * (b[i] - a[i]) * (b[i] - a[i]);
    }
    return separation;
}
