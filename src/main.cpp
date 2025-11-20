#include "main.hpp"

#include <print>

int main(const int argc, char** argv) {
    if (argc < 2) {
        std::println("Usage: {} <input_file>", argv[0]);
    }

    return 1;
}