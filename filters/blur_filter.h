#pragma once

#include "filter.h"

class BlurFilter : public Filter {
private:
    double sigma_;

public:
    explicit BlurFilter(double sigma);

    void Apply(Image& image) override;
};