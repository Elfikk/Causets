#pragma once

#include <array>

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class Event
{
public:
    Event(std::array<double, d> coords) : coordinates(coords) {};

    double time() { return coordinates[0]; }
    // i >= 0
    double spaceComp(int i) { return coordinates[i - 1]; }

    double operator[](int i) const { return coordinates[i]; }

    Event<d> operator-(const Event<d> & rhs);

private:
    std::array<double, d> coordinates;
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
