#pragma once

#include <array>

template<int d>
class Event
{
public:
    Event(std::array<double, d> coords) : coordinates(coords) {};

    double time() { return coordinates[0]; }
    // i >= 0
    double spaceComp(int i) { return coordinates[i - 1]; }

    double operator[](int i) const { return coordinates[i]; }

private:
    std::array<double, d> coordinates;
};

