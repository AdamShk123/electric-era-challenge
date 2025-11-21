#include "calculate.hpp"

#include <print>
#include <sstream>
#include <algorithm>
#include <map>
#include <format>

namespace Calculate {
    auto calculateUptime(const std::string& name) -> std::expected<std::vector<std::string>,ErrorCode> {
        // PROJECT_ROOT macro is defined in CMake to allow user to use relative path when passing argument
        std::ifstream file(std::format("{}{}", PROJECT_ROOT, name));

        // verify that the file has been opened successfully
        if (!file.is_open()) {
            return std::unexpected{ErrorCode::FileNotFound};
        }

        auto stationChargers = parseStationChargers(file);

        if (!stationChargers.has_value()) {
            file.close();
            return std::unexpected{stationChargers.error()};
        }

        auto availabilityReports = parseAvailabilityReports(file);

        if (!availabilityReports.has_value()) {
            file.close();
            return std::unexpected{availabilityReports.error()};
        }

        file.close();

        auto results = produceUptimeResults(stationChargers.value(), availabilityReports.value());

        if (!results.has_value()) {
            return std::unexpected{results.error()};
        }

        return results;
    }

    auto parseStationChargers(std::istream& file) -> std::expected<std::unordered_map<uint32_t, uint32_t>,ErrorCode> {
        std::string line{};

        std::unordered_map<uint32_t, uint32_t> chargerToStation{};

        // skip [Stations] line as it's only a header and doesn't contain any data
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

        // read all data until blank line between the station and uptime sections
        while (std::getline(file, line) && line.size() != 0) {
            // convert line to stream for easier manipulation
            std::stringstream stream{line};

            // get station number
            uint32_t station{};

            if (!(stream >> station)) {
                return std::unexpected{ErrorCode::ParseError};
            };

            uint32_t charger{};

            // get charger numbers
            while (stream >> charger) {
                chargerToStation[charger] = station;
            }

            // check for failure to parse
            if (stream.fail() and !stream.eof()) {
                return std::unexpected{ErrorCode::ParseError};
            }
        }

        return chargerToStation;
    }

    auto parseAvailabilityReports(std::istream& file) -> std::expected<std::unordered_map<uint32_t, std::vector<Uptime>>, ErrorCode> {
        std::string line{};

        std::unordered_map<uint32_t,std::vector<Uptime>> availabilityReports{};

        // skip [Charger Availability Reports] line as it's only a header and doesn't contain any data
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

        // read all data until blank line between the station and uptime sections
        while (std::getline(file, line) && line.size() != 0) {
            // convert line to stream for easier manipulation
            std::stringstream stream{line};

            // get charger number
            uint32_t charger{};

            if (!(stream >> charger)) {
                return std::unexpected{ErrorCode::ParseError};
            };

            Uptime uptime{};

            // get start time
            if (!(stream >> uptime.start)) {
                return std::unexpected{ErrorCode::ParseError};
            };

            // get end time
            if (!(stream >> uptime.end)) {
                return std::unexpected{ErrorCode::ParseError};
            };

            // get whether charger up or down
            std::string value{};

            if (!(stream >> value)) {
                return std::unexpected{ErrorCode::ParseError};
            };

            uptime.up = value == "true";

            availabilityReports[charger].push_back(uptime);
        }

        return availabilityReports;
    }

    auto produceUptimeResults(const std::unordered_map<uint32_t, uint32_t>& chargerToStation, const std::unordered_map<uint32_t, std::vector<Uptime>>& availabilityReports) -> std::expected<std::vector<std::string>,ErrorCode> {
        // Use map instead of unordered_map to keep results sorted by station number
        std::map<uint32_t, uint32_t> results{};

        std::optional<uint64_t> prev = std::nullopt;

        for (const auto &[charger, uptimes] : availabilityReports) {
            uint64_t total{0};
            uint64_t available{0};

            for (const auto &[start, end, up] : uptimes) {
                total += end - start;

                if (prev.has_value()) {
                    total += start - prev.value();
                }

                if (up) {
                    available += end - start;
                }

                prev = end;
            }

            uint32_t station = chargerToStation.at(charger);
            results[station] = static_cast<uint32_t>(static_cast<double>(available) / total * 100);
        }

        // Transform map to vector of strings
        std::vector<std::string> output{};
        output.reserve(results.size());

        std::transform(results.begin(), results.end(), std::back_inserter(output),
            [](const auto& pair) {
                return std::format("{} {}", pair.first, pair.second);
            });

        return output;
    }
} // namespace Calculate
