#include "grayscale_filter.h"

namespace {

constexpr double RedNecessary = 0.299;
constexpr double GreenNecessary = 0.587;
constexpr double BlueNecessary = 0.114;

}  // namespace

void GrayscaleFilter::Apply(Image& image) {
    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            Colour& pixel = image.At(x, y);
            double gray = RedNecessary * pixel.r + GreenNecessary * pixel.g + BlueNecessary * pixel.b;
            pixel = Colour(gray, gray, gray);
        }
    }
}