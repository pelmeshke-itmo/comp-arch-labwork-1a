cmake_minimum_required(VERSION 3.12)

project(
        comp-arch-labwork-1a
        VERSION 1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)

add_executable(example main.cpp)
target_link_libraries(example number)

enable_testing()
add_subdirectory(tests)



