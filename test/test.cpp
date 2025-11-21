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

    const auto expected = std::unordered_map<uint32_t, std::vector<uint32_t>>{
        {0, {1001, 1002}},
        {1, {1003}},
        {2, {1004}}
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

TEST(CalculateTest, CorrectlyParseAvailabilityReports) {
    std::stringstream stream{
        "[Charger Availability Reports]\n"
        "1001 0 50000 true\n"
        "1001 50000 100000 true\n"
        "1002 50000 100000 true\n"
        "1003 25000 75000 false\n"
        "1004 0 50000 true\n"
        "1004 100000 200000 true\n"
    };

    const auto expected = std::unordered_map<uint32_t,std::vector<Calculate::Uptime>>{
        {1001,{{0,50000,true},{50000,100000,true}}},
        {1002,{{50000,100000,true}}},
        {1003,{{25000,75000,false}}},
        {1004,{{0,50000,true},{100000,200000,true}}}
    };

    const auto result = Calculate::parseAvailabilityReports(stream);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), expected);
}

TEST(CalculateTest, CorrectlyProduceResults) {
    const auto stationChargers = std::unordered_map<uint32_t, std::vector<uint32_t>>{
            {0, {1001, 1002}},
            {1, {1003}},
            {2, {1004}}
    };

    const auto availabilityReports = std::unordered_map<uint32_t,std::vector<Calculate::Uptime>>{
            {1001,{{0,50000,true},{50000,100000,true}}},
            {1002,{{50000,100000,true}}},
            {1003,{{25000,75000,false}}},
            {1004,{{0,50000,true},{100000,200000,true}}}
    };

    const auto expected = std::vector<std::string>{
        "0 100",
        "1 0",
        "2 75"
    };

    const auto result = Calculate::produceUptimeResults(stationChargers,availabilityReports);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), expected);
}
