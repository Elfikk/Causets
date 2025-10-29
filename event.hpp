#pragma once

#include <array>

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class Event
{
public:
    Event(std::array<long double, d> coords) : coordinates(coords) {};

    long double time() const { return coordinates[0]; }
    // i >= 0
    long double spaceComp(int i) const { return coordinates[i - 1]; }

    long double operator[](int i) const { return coordinates[i]; }

    Event<d> operator-(const Event<d> & rhs);

    bool operator==(const Event<d> & other);
    bool operator!=(const Event<d> & other) { return !(*this == other); }

private:
    std::array<long double, d> coordinates;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
Event<d> Event<d>::operator-(const Event<d> & rhs)
{
    Event<d> lhs;
    for (int i = 0; i < d; i++)
    {
        lhs.coordinates[i] = this->coordinates[i] - rhs.coordinates[i];
    }
    return lhs;
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
bool Event<d>::operator==(const Event<d> & other)
{
    for (int i = 0; i < d; i++)
    {
        if (coordinates[i] != other[i])
        {
            return false;
        }
    }
    return true;
}
