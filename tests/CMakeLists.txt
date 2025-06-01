cmake_minimum_required(VERSION 3.31.2)
project(RK_Logger_tests_proj)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c*)
add_executable(
    rk_logger_tests
    ${SOURCES}
)
target_include_directories(rk_logger_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(rk_logger_tests PRIVATE
    RK_LOGGER_TESTS_BASE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    RK_LOGGER_TESTS_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}"
)
target_link_libraries(
    rk_logger_tests
    PUBLIC
    GTest::gtest_main
    rk_logger
)

include(GoogleTest)
gtest_discover_tests(rk_logger_tests)
