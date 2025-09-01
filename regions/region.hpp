#pragma once

#include "event.hpp"

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class Region
{
public:
    virtual bool isInside(Event<d> &) = 0;
};
