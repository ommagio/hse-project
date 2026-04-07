#pragma once

#include "filter.h"

class EdgeFilter : public Filter {
private:
    double threshold_;

public:
    explicit EdgeFilter(double threshold);

    void Apply(Image& image) override;
};