#pragma once

#include <expected>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

namespace Calculate {
    enum class ErrorCode {
        FileNotFound,
        ParseError
    };

    std::expected<std::vector<std::string>,ErrorCode> calculateUptime(const std::string& name);
    std::expected<std::unordered_map<int, std::vector<int>>,ErrorCode> parseStationChargers(std::istream &file);
}
