#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

#include "blur_filter.h"

namespace {

constexpr double Radius = 3.0;
constexpr double FormulaValue = 2.0;

std::vector<double> CoeffLine(double sigma) {
    int radius = static_cast<int>(std::ceil(Radius * sigma));
    std::vector<double> line(2 * radius + 1);

    double sum = 0.0;

    for (int i = -radius; i <= radius; ++i) {
        double value = std::exp(-(i * i) / (FormulaValue * sigma * sigma));
        line[i + radius] = value;
        sum += value;
    }

    for (double& value : line) {
        value /= sum;
    }

    return line;
}

}  // namespace

BlurFilter::BlurFilter(double sigma) : sigma_(sigma) {
}

void BlurFilter::Apply(Image& image) {
    std::vector<double> line = CoeffLine(sigma_);
    int radius = static_cast<int>(line.size() / 2);

    Image processing_image = image;

    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            Colour processed_pixel;

            for (int line_range_x = -radius; line_range_x <= radius; ++line_range_x) {

                int neighbour_x =
                    std::clamp(static_cast<int>(x) + line_range_x, 0, static_cast<int>(image.GetWidth()) - 1);

                const Colour& pixel = image.At(static_cast<size_t>(neighbour_x), y);
                double coeff = line[line_range_x + radius];

                processed_pixel.r += pixel.r * coeff;
                processed_pixel.g += pixel.g * coeff;
                processed_pixel.b += pixel.b * coeff;
            }

            processed_pixel.RangeRestriction();
            processing_image.At(x, y) = processed_pixel;
        }
    }

    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            Colour processed_pixel;

            for (int line_range_y = -radius; line_range_y <= radius; ++line_range_y) {

                int neighbour_y =
                    std::clamp(static_cast<int>(y) + line_range_y, 0, static_cast<int>(image.GetHeight()) - 1);

                const Colour& pixel = processing_image.At(x, static_cast<size_t>(neighbour_y));
                double coeff = line[line_range_y + radius];

                processed_pixel.r += pixel.r * coeff;
                processed_pixel.g += pixel.g * coeff;
                processed_pixel.b += pixel.b * coeff;
            }

            processed_pixel.RangeRestriction();
            image.At(x, y) = processed_pixel;
        }
    }
}