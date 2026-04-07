#include <algorithm>

#include "crop_filter.h"

CropFilter::CropFilter(size_t width, size_t height) : width_(width), height_(height) {
}

void CropFilter::Apply(Image& image) {
    size_t new_width = std::min(width_, image.GetWidth());
    size_t new_height = std::min(height_, image.GetHeight());

    Image image_in_process(new_width, new_height);

    for (size_t y = 0; y < new_height; ++y) {
        for (size_t x = 0; x < new_width; ++x) {
            image_in_process.At(x, y) = image.At(x, y);
        }
    }

    image = image_in_process;
}