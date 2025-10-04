#pragma once

#include "event.hpp"

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class Region
{
public:
    virtual bool isInside(const Event<d> &) const = 0;
};
