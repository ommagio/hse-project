#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <fstream>

#include "parser.h"
#include "filters/filter.h"
#include "filters/crop_filter.h"
#include "filters/base_filter.h"
#include "filters/grayscale_filter.h"
#include "filters/negative_filter.h"
#include "filters/sepia_filter.h"
#include "filters/sharpen_filter.h"
#include "filters/edge_filter.h"
#include "filters/blur_filter.h"

ParserCommand CommandLine(int argc, char** argv) {
    if (argc < 3) {
        throw std::runtime_error("Недостаточно аргументов командной строки");
    }

    std::string import_path = argv[1];
    std::string export_path = argv[2];
    std::vector<std::unique_ptr<Filter>> filters;

    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-gs") {
            filters.emplace_back(std::make_unique<GrayscaleFilter>());
        } else if (arg == "-neg") {
            filters.emplace_back(std::make_unique<NegativeFilter>());
        } else if (arg == "-sharp") {
            filters.emplace_back(std::make_unique<SharpenFilter>());
        } else if (arg == "-sepia") {
            filters.emplace_back(std::make_unique<SepiaFilter>());
        } else if (arg == "-crop") {
            if (i + 2 >= argc) {
                throw std::runtime_error("Фильтр -crop требует ширину и высоту изображения");
            }

            int width = std::stoi(argv[++i]);
            int height = std::stoi(argv[++i]);

            if (width <= 0 || height <= 0) {
                throw std::runtime_error("Ширина и высота для -crop должны быть положительными");
            }

            filters.emplace_back(std::make_unique<CropFilter>(static_cast<size_t>(width), static_cast<size_t>(height)));

        } else if (arg == "-edge") {
            if (i + 1 >= argc) {
                throw std::runtime_error("Фильтр -edge требует threshold");
            }

            double threshold = std::stod(argv[++i]);

            if (threshold < 0.0 || threshold > 1.0) {
                throw std::runtime_error("Threshold должен быть в диапазоне [0, 1]");
            }

            filters.emplace_back(std::make_unique<EdgeFilter>(threshold));

        } else if (arg == "-blur") {
            if (i + 1 >= argc) {
                throw std::runtime_error("Фильтр -blur требует sigma");
            }

            double sigma = std::stod(argv[++i]);

            if (sigma <= 0.0) {
                throw std::runtime_error("Sigma для -blur должна быть положительной");
            }

            filters.emplace_back(std::make_unique<BlurFilter>(sigma));

        } else {
            throw std::runtime_error("Неопознанный фильтр: " + arg);
        }
    }

    return ParserCommand(std::move(import_path), std::move(export_path), std::move(filters));
}
