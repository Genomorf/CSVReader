cmake_minimum_required(VERSION 3.0)

project(CSVReader VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(CSVReader src/CSVReader.cpp src/CSVReader.h src/ErrorCheckers.cpp src/ErrorCheckers.h src/main.cpp)
target_link_libraries(CSVReader stdc++fs)
