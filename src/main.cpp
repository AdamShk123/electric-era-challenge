#include "main.hpp"

#include <print>

int main(const int argc, char** argv) {
    // prevent running executable without passing correct number of arguments
    if (argc != 2) {
        std::println(stderr, "Usage: {} <relative path to input file>", argv[0]);
        std::println(stderr, "Call executable from the project's root directory and include relative path to input file");
        std::println(stderr, "Example: ./cmake-build-debug/src/challenge /examples/input_1.txt");
        return 1;
    }

    auto result = Calculate::calculateUptime(std::string(argv[1]));

    if (!result.has_value()) {
        switch (result.error()) {
            case Calculate::ErrorCode::FileNotFound:
                std::println(stderr, "Could not find and read file based on given path: {}", argv[1]);
            case Calculate::ErrorCode::ParseError:
                std::println(stderr, "Failed to parse file");
        }

        return 1;
    }

    for (const auto& line : result.value()) {
        std::println("{}", line);
    }

    return 0;
}