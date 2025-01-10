/**
 * @file log.h
 * @brief The header file for the logger.
 * 
 * This file contains functions, macros, and variables that are used by the logger. See the demonstration directory
 * for an example of its use. Also, see the README for an overview.
 */
#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include <mutex>
#include <sstream>
#include <queue>
#include <thread>
#include <fstream>
#include <condition_variable>
#include <chrono>

#include "logger/log_time.h"

/**
 * @brief Adds a message to the log queue.
 * 
 * This is the primary macro that should be used to log messages. It does not have to be called from the
 * rk::log namespace. It uses the function rk::log::logMessage() in order to add the message to the
 * log queue. It can take any number of arguments for logging. See the demonstration directory for an
 * example.
 */
#define LOG(...) rk::log::logMessage(rk::time::system_clock::now(), __func__, __VA_ARGS__)

/**
 * @brief Sets up the extern variables from the rk::log namespace.
 * 
 * Call this in ONE source file, most likely main() or wherever the central point
 * of the project is. The variables it sets up are used throughout the logging
 * mechanism.
 */
#define LOG_SETUP \
std::mutex rk::log::logQueueMutex; \
std::queue<std::string> rk::log::logQueue; \
std::mutex rk::log::endLoopMtx; \
bool rk::log::endLoop; \
std::ofstream rk::log::logFile("logs.txt"); \
std::condition_variable rk::log::cv; \

/**
 * @brief Verifies whether the log file was created and opened.
 * 
 * Call this after running the LOG_SETUP macro and startLogThread(). This will
 * throw an exception if the file wasn't opened or created.
 */
#define LOG_VERIFY \
if (!rk::log::logFile) { \
    throw -1; \
} \

namespace rk {
namespace log {

extern std::mutex logQueueMutex; 
extern std::queue<std::string> logQueue; /**< Main queue for holding log messages */ 
extern std::mutex endLoopMtx;
extern bool endLoop; /**< Determines whether or not to end the log loop */ 
extern std::ofstream logFile;
extern std::condition_variable cv;

/**
 * @brief Adds a message to the log queue.
 * 
 * This function should not be called by itself. Call it via the LOG macro.
 * 
 * @param time The time that the message was logged.
 * @param funcName The function that this is being called from.
 * @param args The values to construct the message from.
 */
template<typename... Args>
void logMessage(const rk::time::time_point time, const std::string funcName, const Args&... args) {
    std::ostringstream oss;
    oss << rk::time::generateTimeStamp(time); // Prefix the time stamp
    oss << "[" << std::this_thread::get_id() << "][" << funcName << "]"; // Prefix the thread id and function name
    (oss << ... << args);
    std::lock_guard<std::mutex> lock(logQueueMutex);
    logQueue.push(oss.str());
    cv.notify_one();
}

/**
 * @brief Predicate used by the condition variable in the log loop to determine whether to continue waiting or not.
 * 
 * @return Whether or not to resume the log loop.
 */
bool condVarPredicate();

/**
 * @brief Checks the log queue for messages and prints them.
 */
void logQueueLoop();

/**
 * @brief Starts the log thread.
 * 
 * Call this before doing any logging.
 * 
 * @return The thread that was started for the log loop.
 */
std::thread startLogThread();

/**
 * @brief Ends the log thread.
 * 
 * @param std::thread The thread that is running the log loop. Most likely one would pass in the
 * thread that was created in startLogThread().
 */
void endLogThread(std::thread&);

/**
 * @brief Closes the log file.
 */
void closeLogFile();

} // namespace log
} // namespace rk

#endif // #ifndef LOG_H
