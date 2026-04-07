#include "base_filter.h"

namespace {

constexpr double RedNecessary = 0.15;
constexpr double GreenNecessary = 0.15;
constexpr double BlueNecessary = 0.15;

}  // namespace

void BaseFilter::Apply(Image& image) {
    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            Colour& pixel = image.At(x, y);
            double new_colour = RedNecessary * pixel.r + GreenNecessary * pixel.g + BlueNecessary * pixel.b;
            pixel = Colour(new_colour, new_colour, new_colour);
        }
    }
}