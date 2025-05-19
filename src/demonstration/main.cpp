/**
 * @file main.cpp
 * @brief Main file for demonstrating the use of the logger.
 */
#include <thread>
#include <random>

#include <rk_logger/logger.h>
#include "file1.h"

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
    if (otherThread.joinable()) {
        otherThread.join();
    }

    // Print messages on a timer
    constexpr size_t MAX_ITERATIONS = 15;
    size_t timerCount = 0;
    std::thread timerThread([&MAX_ITERATIONS, &timerCount] () {
        while (timerCount < MAX_ITERATIONS) {
            RK_LOG("Printing a message every 3 seconds. Timer messages remaining: ", MAX_ITERATIONS - timerCount, "\n");
            std::this_thread::sleep_for(std::chrono::seconds(3));
            timerCount++;
        }
    });

    // Generate some random events
    std::random_device rd;
    std::mt19937 gen(rd());
    double randomDuration;
    constexpr size_t MAX_RANDOM_EVENTS = 15;
    size_t randomEventsCount = 0;
    std::thread randomEvents([&gen, &randomDuration, &randomEventsCount, &MAX_RANDOM_EVENTS] () {
        while (randomEventsCount < MAX_RANDOM_EVENTS) {
            std::uniform_real_distribution<> dist(0, 5);
            randomDuration = dist(gen);
            RK_LOG("Random event happened. Printing a log for it. Random events remaining: ", MAX_RANDOM_EVENTS - randomEventsCount, "\n");
            std::this_thread::sleep_for(std::chrono::duration<double>(randomDuration));
            randomEventsCount++;
        }
    });

    if (timerThread.joinable()) {
        timerThread.join();
    }
    if (randomEvents.joinable()) {
        randomEvents.join();
    }

    // Call this at the end of the program to shutdown the logger
    rk::log::stopLogger(std::move(logThread));

    return 0;
}
