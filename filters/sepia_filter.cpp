#include "sepia_filter.h"

namespace {

constexpr double PixelRR = 0.393;
constexpr double PixelRG = 0.769;
constexpr double PixelRB = 0.189;
constexpr double PixelGR = 0.349;
constexpr double PixelGG = 0.686;
constexpr double PixelGB = 0.168;
constexpr double PixelBR = 0.272;
constexpr double PixelBG = 0.534;
constexpr double PixelBB = 0.131;

}  // namespace

void SepiaFilter::Apply(Image& image) {
    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            Colour& pixel = image.At(x, y);

            double r = pixel.r;
            double g = pixel.g;
            double b = pixel.b;

            pixel.r = PixelRR * r + PixelRG * g + PixelRB * b;
            pixel.g = PixelGR * r + PixelGG * g + PixelGB * b;
            pixel.b = PixelBR * r + PixelBG * g + PixelBB * b;

            pixel.RangeRestriction();
        }
    }
}