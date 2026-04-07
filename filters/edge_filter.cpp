#include <algorithm>

#include "edge_filter.h"
#include "grayscale_filter.h"

EdgeFilter::EdgeFilter(double threshold) : threshold_(threshold) {
}

void EdgeFilter::Apply(Image& image) {
    GrayscaleFilter grayscale;
    grayscale.Apply(image);

    Image source = image;

    const double matrix[3][3] = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};

    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            Colour processed_pixel;

            for (int matrix_range_y = -1; matrix_range_y <= 1; ++matrix_range_y) {
                for (int matrix_range_x = -1; matrix_range_x <= 1; ++matrix_range_x) {

                    int neighbour_x =
                        std::clamp(static_cast<int>(x) + matrix_range_x, 0, static_cast<int>(image.GetWidth()) - 1);

                    int neighbour_y =
                        std::clamp(static_cast<int>(y) + matrix_range_y, 0, static_cast<int>(image.GetHeight()) - 1);

                    const Colour& pixel = source.At(static_cast<size_t>(neighbour_x), static_cast<size_t>(neighbour_y));
                    double coeff = matrix[matrix_range_y + 1][matrix_range_x + 1];

                    processed_pixel.r += pixel.r * coeff;
                    processed_pixel.g += pixel.g * coeff;
                    processed_pixel.b += pixel.b * coeff;
                }
            }

            processed_pixel.RangeRestriction();

            if (processed_pixel.r > threshold_) {
                image.At(x, y) = Colour(1.0, 1.0, 1.0);
            } else {
                image.At(x, y) = Colour(0.0, 0.0, 0.0);
            }
        }
    }
}