#pragma once

#include "filter.h"

class SepiaFilter : public Filter {
public:
    void Apply(Image& image) override;
};