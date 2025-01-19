/**
 * @file main.cpp
 * @brief Main file for demonstrating the use of the logger.
 */
#include "rk_logger/log.h"
#include "file1.h"

#include <thread>

int main() {
    // Call this function to start the logger. Save the logging thread for when the program ends.
    std::thread logThread = rk::log::startLogger();

    // Log a message from main
    RK_LOG("Inside main\n");

    // Log a message with some variables
    int number = 10;
    std::string name = "Ryan";
    RK_LOG("The number is ", number, ". The name is ", name, "\n");

    // Log a message from another file
    file1::myFunc();

    // Log a message from another thread. This should print a different thread id and function name
    std::thread otherThread([] () {
        RK_LOG("Inside other thread\n");
    });
    otherThread.join();

    // Call this at the end of the program to shutdown the logger
    rk::log::stopLogger(std::move(logThread));

    return 0;
}
