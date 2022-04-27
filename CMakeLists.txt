cmake_minimum_required(VERSION 3.20)
project(SFML_LCD)

set(CMAKE_CXX_STANDARD 14)

add_executable(SFML main.cpp vrEmuLcd.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(c:/SFML/include)
target_link_libraries(SFML sfml-system sfml-window sfml-graphics sfml-audio)
