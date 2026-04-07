#pragma once

#include <algorithm>

class Colour {
public:
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    Colour() = default;

    Colour(double r, double g, double b) : r(r), g(g), b(b) {
    }

    void RangeRestriction() {
        r = std::clamp(r, 0.0, 1.0);
        g = std::clamp(g, 0.0, 1.0);
        b = std::clamp(b, 0.0, 1.0);
    }
};