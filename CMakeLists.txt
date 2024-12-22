cmake_minimum_required(VERSION 3.31.2)
project(rk_logger)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_library(rk_logger ${PROJECT_SOURCE_DIR}/src/log.cpp)
target_include_directories(rk_logger PRIVATE ${PROJECT_SOURCE_DIR}/include)
