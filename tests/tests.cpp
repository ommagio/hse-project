#include <cmath>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "colour.h"
#include "image.h"

#include "filters/blur_filter.h"
#include "filters/crop_filter.h"
#include "filters/edge_filter.h"
#include "filters/grayscale_filter.h"
#include "filters/negative_filter.h"
#include "filters/sepia_filter.h"

namespace {

constexpr double MinusOne = -1.0;
constexpr double Half = 0.5;
constexpr double Two = 2.0;
constexpr double Eps = 1e-6;

constexpr double PointOne = 0.1;
constexpr double PointTwo = 0.2;
constexpr double PointThree = 0.3;
constexpr double PointFour = 0.4;
constexpr double PointSix = 0.6;
constexpr double PointEight = 0.8;
constexpr double Quarter = 0.25;
constexpr double ThreeQuarters = 0.75;

constexpr double GrayscaleExpected = 0.299;
constexpr double SepiaExpectedR = 0.393;
constexpr double SepiaExpectedG = 0.349;
constexpr double SepiaExpectedB = 0.272;

constexpr int CropHeight = 5;

void Check(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void CheckNear(double actual, double expected, double eps, const std::string& message) {
    if (std::fabs(actual - expected) > eps) {
        throw std::runtime_error(message);
    }
}

void TestColour() {
    Colour colour(MinusOne, Half, Two);
    colour.RangeRestriction();

    CheckNear(colour.r, 0.0, Eps, "Colour colour.r error");
    CheckNear(colour.g, Half, Eps, "Colour colour.g error");
    CheckNear(colour.b, 1.0, Eps, "Colour colour.b error");
}

void TestGrayscale() {
    Image image(1, 1);
    image.At(0, 0) = Colour(1.0, 0.0, 0.0);

    GrayscaleFilter filter;
    filter.Apply(image);

    CheckNear(image.At(0, 0).r, GrayscaleExpected, Eps, "Grayscale colour.r error");
    CheckNear(image.At(0, 0).g, GrayscaleExpected, Eps, "Grayscale colour.g error");
    CheckNear(image.At(0, 0).b, GrayscaleExpected, Eps, "Grayscale colour.b error");
}

void TestNegative() {
    Image image(1, 1);
    image.At(0, 0) = Colour(PointTwo, PointFour, PointEight);

    NegativeFilter filter;
    filter.Apply(image);

    CheckNear(image.At(0, 0).r, PointEight, Eps, "Negative colour.r error");
    CheckNear(image.At(0, 0).g, PointSix, Eps, "Negative colour.g error");
    CheckNear(image.At(0, 0).b, PointTwo, Eps, "Negative colour.b error");
}

void TestCrop() {
    Image image(3, 2);
    image.At(0, 0) = Colour(PointOne, 0.0, 0.0);
    image.At(1, 0) = Colour(PointTwo, 0.0, 0.0);
    image.At(2, 0) = Colour(PointThree, 0.0, 0.0);
    image.At(0, 1) = Colour(PointFour, 0.0, 0.0);
    image.At(1, 1) = Colour(Half, 0.0, 0.0);
    image.At(2, 1) = Colour(PointSix, 0.0, 0.0);

    CropFilter filter(2, CropHeight);
    filter.Apply(image);

    Check(image.GetWidth() == 2, "Crop width error");
    Check(image.GetHeight() == 2, "Crop height error");
    CheckNear(image.At(1, 1).r, Half, Eps, "Crop pixel error");
}

void TestEdgeBlack() {
    Image image(3, 3);
    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            image.At(x, y) = Colour(0.0, 0.0, 0.0);
        }
    }

    EdgeFilter filter(PointOne);
    filter.Apply(image);

    CheckNear(image.At(1, 1).r, 0.0, Eps, "Edge colour.r error");
}

void TestBlur() {
    Image image(4, 4);
    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            image.At(x, y) = Colour(Quarter, Half, ThreeQuarters);
        }
    }

    BlurFilter filter(1.0);
    filter.Apply(image);

    CheckNear(image.At(2, 2).r, Quarter, Eps, "Blur colour.r error");
    CheckNear(image.At(2, 2).g, Half, Eps, "Blur colour.g error");
    CheckNear(image.At(2, 2).b, ThreeQuarters, Eps, "Blur colour.b error");
}

void TestSepia() {
    Image image(1, 1);
    image.At(0, 0) = Colour(1.0, 0.0, 0.0);

    SepiaFilter filter;
    filter.Apply(image);

    CheckNear(image.At(0, 0).r, SepiaExpectedR, Eps, "Sepia colour.r error");
    CheckNear(image.At(0, 0).g, SepiaExpectedG, Eps, "Sepia colour.g error");
    CheckNear(image.At(0, 0).b, SepiaExpectedB, Eps, "Sepia colour.b error");
}

}  // namespace

int main() {
    try {
        TestColour();
        TestGrayscale();
        TestNegative();
        TestCrop();
        TestEdgeBlack();
        TestBlur();
        TestSepia();

        std::cout << "Тесты пройдены" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Поймана ошибка: " << e.what() << std::endl;
        return 1;
    }
}