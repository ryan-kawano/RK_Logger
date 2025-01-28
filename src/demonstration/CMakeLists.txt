cmake_minimum_required(VERSION 3.31.2)
project(rk_logger_demonstration)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(RK_LOGGER_DEMO_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "The directory of the RK Logger demonstration project")

file(GLOB RK_DEMO_SOURCES ${RK_LOGGER_DEMO_DIR}/*.cpp)
add_executable(rk_logger_demonstration ${RK_DEMO_SOURCES})
target_include_directories(rk_logger_demonstration PRIVATE ${RK_LOGGER_DEMO_DIR})
target_link_libraries(rk_logger_demonstration PUBLIC rk_logger)
