#pragma once

#include "spacetime.hpp"

#include <limits>

//---------------------------------------------------------------------------------------------------------------------

struct ads_spacetime: public generic_spacetime {};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class AdS: public Spacetime<d>
{
public:
    typedef ads_spacetime spacetime_class;

    AdS(double lowerR) : R0(lowerR) {}
    CausalRelation causalRelation(const Event<d> & a, const Event<d> & b) const override;

    double getLowerBound(int i) const override;
    double getUpperBound(int i) const override { return std::numeric_limits<double>::max(); }

    double getR0() { return R0; }

private:
    // std function that takes 1st space component of Event and returns an
    // array for the local metric

    double R0 = 0.1;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
struct spacetime_traits<AdS<d>>
{
    typedef typename AdS<d>::spacetime_class spacetime_class;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
double AdS<d>::getLowerBound(int i) const
{
    if (i == 1)
    {
        return R0;
    }
    return std::numeric_limits<double>::lowest();
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
CausalRelation AdS<d>::causalRelation(const Event<d> & a, const Event<d> & b) const
{
    auto interval = - (b[0] - a[0]) * (b[0] - a[0]);
    for (int i = 1; i < d; i++)
    {
        interval += (b[i] - a[i]) * (b[i] - a[i]);
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
