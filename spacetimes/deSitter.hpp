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

    DeSitter(std::array<double, 2 * d> boundaries) : bounds(boundaries) {}
    CausalRelation causalRelation(const Event<d> & a, const Event<d> & b) const override;
    double getLowerBound(int i) const override { return bounds[2 * i]; }
    double getUpperBound(int i) const override { return bounds[2 * i + 1]; }

private:
    std::array<double, 2 * d> bounds;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
struct spacetime_traits<DeSitter<d>>
{
    typedef typename DeSitter<d>::spacetime_class spacetime_class;
};

//---------------------------------------------------------------------------------------------------------------------

template<>
CausalRelation DeSitter<2>::causalRelation(const Event<2> & a, const Event<2> & b) const
{
    // Must Be Using Conformal Coordinates
    const auto separation = (b[1] - a[1]) * (b[1] - a[1]) - (b[0] - a[0]) * (b[0] - a[0]);
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
