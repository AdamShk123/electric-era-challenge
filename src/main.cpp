#include "main.hpp"

#include <fstream>
#include <print>
#include <unordered_map>
#include <vector>
#include <sstream>

int main(const int argc, char** argv) {
    // prevent running executable without passing correct number of arguments
    if (argc != 2) {
        std::println(stderr, "Usage: {} <relative path to input file>", argv[0]);
        std::println(stderr, "Call executable from the project's root directory and include relative path to input file");
        std::println(stderr, "Example: ./cmake-build-debug/src/challenge /examples/input_1.txt");
        return 1;
    }

    // PROJECT_ROOT macro is defined in CMake to allow user to use relative path when passing argument
    std::ifstream file(std::string(PROJECT_ROOT) + std::string(argv[1]));

    // verify that the file has been opened successfully
    if (!file.is_open()) {
        std::println(stderr, "Could not open file: {}", argv[1]);
        return 1;
    }

    std::println("Successfully opened file: {}", argv[1]);

    std::string line{};

    std::unordered_map<int, std::vector<int>> stationChargers{};

    // skip [Stations] line as it's only a header and doesn't contain any data
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    // read all data until blank line between the station and uptime sections
    while (std::getline(file, line) && line.size() != 0) {
        std::println("{}",line);

        std::stringstream stream{line};

        std::vector<std::string> tokens{};

        // get station number
        int station{};

        if (!(stream >> station)) {
            std::println(stderr, "Could not read station: {}", line);
        };

        int charger{};

        // get charger numbers
        while (!(stream >> charger)) {
            stationChargers[station].push_back(charger);
        }

        if (stream.fail()) {
            std::println(stderr, "Could not read charger: {}", line);
        }
    }

    file.close();

    return 0;
}