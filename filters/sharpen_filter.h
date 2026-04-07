#pragma once

#include "filter.h"

class SharpenFilter : public Filter {
public:
    void Apply(Image& image) override;
};