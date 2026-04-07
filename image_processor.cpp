#include <exception>
#include <iostream>

#include "bmp.h"
#include "parser.h"

int main(int argc, char** argv) {
    try {
        if (argc == 1) {
            std::cout << "Обратите внимание на структуру Вашей команды" << std::endl;
            std::cout << "./image_processor <input.bmp> <output.bmp> [filters]" << std::endl;
            return 0;
        }

        ParserCommand command = CommandLine(argc, argv);

        Image image = ReadBMP(command.GetImportPath());

        for (const auto& filter : command.GetFilters()) {
            filter->Apply(image);
        }

        WriteBMP(command.GetExportPath(), image);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}