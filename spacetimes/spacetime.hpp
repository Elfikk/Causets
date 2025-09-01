#pragma once

template<int d>
class Event;

//---------------------------------------------------------------------------------------------------------------------

enum class CausalRelation
{
    CausalFuture,
    CausalPast,
    TimeLike,
    Spacelike,
};

//---------------------------------------------------------------------------------------------------------------------

template<typename SpacetimeT>
struct spacetime_traits
{
    typedef typename SpacetimeT::spacetime_class spacetime_class;
};

//---------------------------------------------------------------------------------------------------------------------

// Dumb ass - no virtuals in templated programming...
template<int d>
class Spacetime
{
public:
    typedef Spacetime<d> spacetime_class;

    virtual ~Spacetime() {}
    virtual CausalRelation causalRelation(const Event<d> & a, const Event<d> & b) const = 0;
    virtual double getLowerBound(int i) const = 0;
    virtual double getUpperBound(int i) const = 0;
};
