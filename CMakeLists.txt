cmake_minimum_required(VERSION 3.29)
project(Sudoku)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "C:/Program Files/SFML/SFML-2.5.0/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(Sudoku main.cpp
        tests_main.cpp
)

target_link_libraries(Sudoku sfml-graphics sfml-window sfml-system)