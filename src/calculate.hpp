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
    auto parseStationChargers(std::istream &file) -> std::expected<std::unordered_map<uint32_t, std::vector<uint32_t>>,ErrorCode>;
    auto parseAvailabilityReports(std::istream &file) -> std::expected<std::unordered_map<uint32_t, std::vector<Uptime>>, ErrorCode>;
    auto produceUptimeResults(const std::unordered_map<uint32_t, std::vector<uint32_t>>& stationChargers, const std::unordered_map<uint32_t, std::vector<Uptime>>& availabilityReports) -> std::expected<std::vector<std::string>,ErrorCode>;
    auto mergeIntervals(std::vector<Uptime>&& intervals) -> std::vector<Uptime>;
    auto resolvePercentage(const std::vector<Uptime>& merged) -> uint32_t;
}
