#include "negative_filter.h"

void NegativeFilter::Apply(Image& image) {
    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            Colour& pixel = image.At(x, y);
            pixel.r = 1.0 - pixel.r;
            pixel.g = 1.0 - pixel.g;
            pixel.b = 1.0 - pixel.b;
        }
    }
}