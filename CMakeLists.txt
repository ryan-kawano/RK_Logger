cmake_minimum_required(VERSION 3.31.2)
project(rk_logger)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
include(CTest)

set(RK_LOGGER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "The root-level of the RK Logger project")

file(GLOB SOURCES ${RK_LOGGER_SOURCE_DIR}/src/*.cpp)
add_library(rk_logger STATIC ${SOURCES})
target_include_directories(rk_logger PUBLIC ${RK_LOGGER_SOURCE_DIR}/include)

add_subdirectory(${RK_LOGGER_SOURCE_DIR}/src/demonstration)
add_subdirectory(${RK_LOGGER_SOURCE_DIR}/tests)
