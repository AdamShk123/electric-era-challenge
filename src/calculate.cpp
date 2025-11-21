#include "calculate.hpp"

#include <print>
#include <sstream>

namespace Calculate {
    std::expected<std::vector<std::string>,ErrorCode>  calculateUptime(const std::string& name) {
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

        file.close();

        return {};
    }

    std::expected<std::unordered_map<int, std::vector<int>>,ErrorCode> parseStationChargers(std::istream& file) {
        std::string line{};

        std::unordered_map<int, std::vector<int>> stationChargers{};

        // skip [Stations] line as it's only a header and doesn't contain any data
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

        // read all data until blank line between the station and uptime sections
        while (std::getline(file, line) && line.size() != 0) {
            // convert line to stream for easier manipulation
            std::stringstream stream{line};

            // get station number
            int station{};

            if (!(stream >> station)) {
                return std::unexpected{ErrorCode::ParseError};
            };

            int charger{};

            // get charger numbers
            while (stream >> charger) {
                stationChargers[station].push_back(charger);
            }

            // check for failure to parse
            if (stream.fail() and !stream.eof()) {
                return std::unexpected{ErrorCode::ParseError};
            }
        }

        return stationChargers;
    }
}