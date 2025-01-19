/**
 * @file main.cpp
 * @brief Main file for demonstrating the use of the logger.
 */
#include "logger/log.h"
#include "file1.h"

#include <thread>

// Call this first to setup the extern variables
LOG_SETUP

int main() {
    // Call this second to read config settings from a file (if it exists) and update the internal config
    rk::config::getLoggingConfig();
    rk::time::updateTimeStampFuncs();

    // Call this third to start the log thread
    std::thread logThread = rk::log::startLogThread();

    // Call this fourth to verify that the log file was created and opened
    LOG_VERIFY

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

    // Call these at the end of the program to end the log thread and close the log file
    rk::log::endLogThread(logThread);
    rk::log::closeLogFile();

    return 0;
}
