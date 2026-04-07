#pragma once

#include "filter.h"

class BaseFilter : public Filter {
public:
    void Apply(Image& image) override;
};