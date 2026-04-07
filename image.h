#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

#include "colour.h"

class Image {
private:
    size_t width_ = 0;
    size_t height_ = 0;
    std::vector<Colour> pixels_;

public:
    Image() = default;

    Image(size_t width, size_t height) : width_(width), height_(height), pixels_(width * height) {
    }

    bool Empty() const {
        return width_ == 0 || height_ == 0;
    }

    size_t GetWidth() const {
        return width_;
    }

    size_t GetHeight() const {
        return height_;
    }

    Colour& At(size_t x, size_t y) {
        if (x >= width_ || y >= height_) {
            throw std::out_of_range("Координаты не существуют");
        }
        return pixels_[y * width_ + x];
    }

    const Colour& At(size_t x, size_t y) const {
        if (x >= width_ || y >= height_) {
            throw std::out_of_range("Координаты не существуют");
        }
        return pixels_[y * width_ + x];
    }
};