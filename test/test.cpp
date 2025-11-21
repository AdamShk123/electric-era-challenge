#include <gtest/gtest.h>

#include "../src/calculate.hpp"

TEST(CalculateTest, NoFileFound) {
    const std::string nonExistentPath{"/examples/input_3.txt"};

    const auto result = Calculate::calculateUptime(nonExistentPath);

    ASSERT_TRUE(!result.has_value());
    EXPECT_EQ(result.error(), Calculate::ErrorCode::FileNotFound);
}

TEST(CalculateTest, FileExistsAndResultCanBeCalculated) {
    const std::string nonExistentPath{"/examples/input_1.txt"};

    const auto expected = std::vector<std::string>{
        "0 100",
        "1 0",
        "2 75"
    };

    const auto result = Calculate::calculateUptime(nonExistentPath);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(expected, expected);
}

TEST(CalculateTest, CorrectlyParseStationChargers) {
    std::stringstream stream{
        "[Stations]\n"
        "0 1001 1002\n"
        "1 1003\n"
        "2 1004\n"
    };

    const auto expected = std::unordered_map<int, std::vector<int>>{
        {0,{1001,1002}},
        {1,{1003}},
        {2,{1004}}
    };

    const auto result = Calculate::parseStationChargers(stream);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), expected);
}

TEST(CalculateTest, ErrorWhileParsingStationChargersDueToNonAllowedCharacter) {
    std::stringstream stream{
        "[Stations]\n"
        "0 100A\n"
    };

    const auto result = Calculate::parseStationChargers(stream);

    ASSERT_TRUE(!result.has_value());
    EXPECT_EQ(result.error(), Calculate::ErrorCode::ParseError);
}
