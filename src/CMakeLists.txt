add_executable(challenge)

target_compile_options(challenge PRIVATE -Wall -Werror)

target_sources(challenge PRIVATE
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)