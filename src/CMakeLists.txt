add_executable(challenge)

target_compile_options(challenge PRIVATE -Wall -Werror)

target_compile_definitions(challenge PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

target_sources(challenge PRIVATE
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)