#pragma once

#include <expected>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <cstdint>

namespace Calculate {
    enum class ErrorCode {
        FileNotFound,
        ParseError
    };

    struct Uptime {
        uint64_t start;
        uint64_t end;
        bool up;

        bool operator==(const Uptime& other) const {
            return start == other.start && end == other.end && up == other.up;
        }
    };

    auto calculateUptime(const std::string& name) -> std::expected<std::vector<std::string>,ErrorCode> ;
    auto parseStationChargers(std::istream &file) -> std::expected<std::unordered_map<uint32_t, uint32_t>,ErrorCode>;
    auto parseAvailabilityReports(std::istream &file) -> std::expected<std::unordered_map<uint32_t, std::vector<Uptime>>, ErrorCode>;
    auto produceUptimeResults(const std::unordered_map<uint32_t, uint32_t>& chargerToStation, const std::unordered_map<uint32_t, std::vector<Uptime>>& uptimes) -> std::expected<std::vector<std::string>,ErrorCode>;
}
