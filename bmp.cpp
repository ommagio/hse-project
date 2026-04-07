#include <algorithm>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "bmp.h"

namespace {

#pragma pack(push, 1)

constexpr double ByteScale = 255.0;
constexpr double RoundBias = 0.5;
constexpr uint16_t BmpSignature = 0x4D42;
constexpr uint32_t InfoHeaderSize = 40;
constexpr uint16_t BmpBitsPerPixel = 24;

struct BMPFileHeader {
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
};

struct BMPInfoHeader {
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerM;
    int32_t yPixelsPerM;
    uint32_t coloursUsed;
    uint32_t coloursImportant;
};

#pragma pack(pop)

double ToDouble(uint8_t value) {
    return static_cast<double>(value) / ByteScale;
}

uint8_t ToByte(double value) {
    value = std::clamp(value, 0.0, 1.0);
    return static_cast<uint8_t>(value * ByteScale + RoundBias);
}

size_t GetPadding(size_t width) {
    return (4 - (width * 3) % 4) % 4;
}

}  // namespace

Image ReadBMP(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть Ваш BMP файл");
    }

    BMPFileHeader file_header;
    BMPInfoHeader info_header;

    file.read(reinterpret_cast<char*>(&file_header), sizeof(BMPFileHeader));
    file.read(reinterpret_cast<char*>(&info_header), sizeof(BMPInfoHeader));

    if (!file) {
        throw std::runtime_error("Не удалось прочитать заголовки Вашего файла");
    }

    if (file_header.signature != BmpSignature) {
        throw std::runtime_error("Ваш файл не является BMP");
    }

    if (info_header.headerSize != InfoHeaderSize) {
        throw std::runtime_error("У Вашего файла неподдерживаемая структура");
    }

    if (info_header.planes != 1) {
        throw std::runtime_error("У Вашего файла некорректное значение количества плоскостей");
    }

    if (info_header.bitsPerPixel != BmpBitsPerPixel) {
        throw std::runtime_error("Поддерживается исключительно 24 битный формат");
    }

    if (info_header.compression != 0) {
        throw std::runtime_error("BMP файлы со сжатием не поддерживаются");
    }

    if (info_header.width <= 0 || info_header.height <= 0) {
        throw std::runtime_error("Ваш файл имеет некорректные размеры");
    }

    const size_t width = static_cast<size_t>(info_header.width);
    const size_t height = static_cast<size_t>(info_header.height);

    Image image(width, height);

    const size_t padding = GetPadding(width);

    for (size_t row = 0; row < height; ++row) {
        const size_t y = height - 1 - row;

        for (size_t x = 0; x < width; ++x) {
            uint8_t b = 0;
            uint8_t g = 0;
            uint8_t r = 0;

            file.read(reinterpret_cast<char*>(&b), 1);
            file.read(reinterpret_cast<char*>(&g), 1);
            file.read(reinterpret_cast<char*>(&r), 1);

            if (!file) {
                throw std::runtime_error("Ошибка чтения пиксельных данных вашего Файла");
            }

            image.At(x, y) = Colour(ToDouble(r), ToDouble(g), ToDouble(b));
        }

        file.ignore(static_cast<std::streamsize>(padding));
        if (!file) {
            throw std::runtime_error("Ошибка padding");
        }
    }

    return image;
}  // read

void WriteBMP(const std::string& path, const Image& image) {
    if (image.Empty()) {
        throw std::runtime_error("Изображение пусто");
    }

    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть выходной BMP файл");
    }

    const uint32_t width = static_cast<uint32_t>(image.GetWidth());
    const uint32_t height = static_cast<uint32_t>(image.GetHeight());
    const size_t padding = GetPadding(width);
    const uint32_t row_size = width * 3 + static_cast<uint32_t>(padding);
    const uint32_t image_size = row_size * height;

    BMPFileHeader file_header;
    file_header.signature = BmpSignature;
    file_header.fileSize = static_cast<uint32_t>(sizeof(BMPFileHeader) + sizeof(BMPInfoHeader)) + image_size;
    file_header.reserved = 0;
    file_header.dataOffset = static_cast<uint32_t>(sizeof(BMPFileHeader) + sizeof(BMPInfoHeader));

    BMPInfoHeader info_header;
    info_header.headerSize = InfoHeaderSize;
    info_header.width = static_cast<int32_t>(width);
    info_header.height = static_cast<int32_t>(height);
    info_header.planes = 1;
    info_header.bitsPerPixel = BmpBitsPerPixel;
    info_header.compression = 0;
    info_header.imageSize = image_size;
    info_header.xPixelsPerM = 0;
    info_header.yPixelsPerM = 0;
    info_header.coloursUsed = 0;
    info_header.coloursImportant = 0;

    file.write(reinterpret_cast<const char*>(&file_header), sizeof(BMPFileHeader));
    file.write(reinterpret_cast<const char*>(&info_header), sizeof(BMPInfoHeader));

    if (!file) {
        throw std::runtime_error("Не удалось записать BMP данные");
    }

    std::vector<uint8_t> pad(padding, 0);

    for (size_t row = 0; row < image.GetHeight(); ++row) {
        const size_t y = image.GetHeight() - 1 - row;

        for (size_t x = 0; x < image.GetWidth(); ++x) {
            const Colour& pixel = image.At(x, y);

            const uint8_t b = ToByte(pixel.b);
            const uint8_t g = ToByte(pixel.g);
            const uint8_t r = ToByte(pixel.r);

            file.write(reinterpret_cast<const char*>(&b), 1);
            file.write(reinterpret_cast<const char*>(&g), 1);
            file.write(reinterpret_cast<const char*>(&r), 1);

            if (!file) {
                throw std::runtime_error("Не удалось записать пиксели изображения");
            }
        }

        if (!pad.empty()) {
            file.write(reinterpret_cast<const char*>(pad.data()), static_cast<std::streamsize>(pad.size()));
            if (!file) {
                throw std::runtime_error("Ошибка записи padding");
            }
        }
    }
}  // write