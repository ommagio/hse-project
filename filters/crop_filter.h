#pragma once

#include <cstddef>

#include "filter.h"

class CropFilter : public Filter {
private:
    size_t width_;
    size_t height_;

public:
    CropFilter(size_t width, size_t height);

    void Apply(Image& image) override;
};