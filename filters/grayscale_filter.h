#pragma once

#include "filter.h"

class GrayscaleFilter : public Filter {
public:
    void Apply(Image& image) override;
};