#pragma once

#include "spacetime.hpp"

#include <limits>

//---------------------------------------------------------------------------------------------------------------------

struct de_sitter_spacetime: public generic_spacetime {};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class DeSitter: public Spacetime<d>
{
public:
    typedef de_sitter_spacetime spacetime_class;

    DeSitter(std::array<long double, 2 * d> boundaries, long double _maxZeta = 1)
    : bounds(boundaries), maxZeta(_maxZeta) {}

    CausalRelation causalRelation(const Event<d> & a, const Event<d> & b) const override;
    long double getLowerBound(int i) const override { return bounds[2 * i]; }
    long double getUpperBound(int i) const override { return bounds[2 * i + 1]; }

private:
    std::array<long double, 2 * d> bounds;
    long double maxZeta;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
struct spacetime_traits<DeSitter<d>>
{
    typedef typename DeSitter<d>::spacetime_class spacetime_class;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
CausalRelation DeSitter<d>::causalRelation(const Event<d> & a, const Event<d> & b) const
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
