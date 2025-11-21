include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test test.cpp)

target_compile_options(test PRIVATE -Wall -Wextra -Werror)

target_link_libraries(test GTest::gtest GTest::gtest_main)

target_compile_definitions(test PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

target_sources(test PRIVATE
    ${PROJECT_SOURCE_DIR}/test/test.cpp
    ${PROJECT_SOURCE_DIR}/src/calculate.cpp
)