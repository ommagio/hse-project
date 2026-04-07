#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "filters/filter.h"

class ParserCommand {
private:
    std::string import_path_;
    std::string export_path_;
    std::vector<std::unique_ptr<Filter>> filters_;

public:
    ParserCommand(std::string import_path, std::string export_path, std::vector<std::unique_ptr<Filter>> filters)
        : import_path_(std::move(import_path)), export_path_(std::move(export_path)), filters_(std::move(filters)) {
        if (import_path_.empty() || export_path_.empty()) {
            throw std::runtime_error("Пожалуйста, заполните поля пути");
        }
    }

    const std::string& GetImportPath() const {
        return import_path_;
    }

    const std::string& GetExportPath() const {
        return export_path_;
    }

    const std::vector<std::unique_ptr<Filter>>& GetFilters() const {
        return filters_;
    }
};

ParserCommand CommandLine(int argc, char** argv);