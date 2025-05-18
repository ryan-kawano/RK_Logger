/**
 * @file log.h
 * @brief Header for the logger.
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

#include <rk_logger/config.h>
#include <rk_logger/log_time.h>

/**
 * @brief Adds a message to the log queue.
 * 
 * This is the primary macro that should be used to log messages. It uses an internal function to add the message to the
 * log queue. It can take any number of arguments for logging. See the demonstration directory for an example.
 */
/**
 * @brief Verifies whether the log file was created and opened.
 */
#define LOG_VERIFY \
if (!rk::log::logFile) { \
    std::cout << "Unable to open output log file\n"; \
    throw -1; \
} \
#define RK_LOG(...) rk::log_internal::logMessage(rk::time_internal::system_clock::now(), __func__, __VA_ARGS__)

namespace rk {
namespace log {

/**
 * @brief The main function that starts the logger. Sets up various things like configs, logging threads, etc.
 * Call this first, before logging any messages.
 * 
 * @param configPath The path to the config file.
 * @return The thread that is running the log loop.
 */
std::thread startLogger(const std::filesystem::path& configPath = std::filesystem::current_path()/rk::config::CONFIG_FILE_NAME);

/**
 * @brief Ends the logger. Call this at the end before the program ends.
 * 
 * @param std::thread The thread that was running the log loop.
 */
void stopLogger(std::thread);

} // namespace log
} // namespace rk

namespace rk {
namespace log_internal {

extern std::mutex logQueueMutex;
extern std::queue<std::string> logQueue; /**< Main queue for holding log messages */
extern std::mutex endLoopMtx;
extern bool endLogLoop;
extern std::ofstream logFile;
extern std::condition_variable logQueueCv;

/**
 * @brief Adds a message to the log queue.
 * 
 * This function should not be called by itself. Call it via the RK_LOG macro.
 * 
 * @param time The time that the message was logged.
 * @param funcName The function that this is being called from.
 * @param args The values to construct the message from.
 */
template<typename... Args>
void logMessage(const rk::time_internal::time_point time, const std::string funcName, const Args&... args) {
    std::ostringstream oss;
    oss << rk::time_internal::generateTimeStamp(time); // Prefix the timestamp
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
 * @brief Starts the log thread. This should be called before doing any logging.
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
void endLogThread(std::thread);

/**
 * @brief Opens the log file.
 */
void openLogFile();

/**
 * @brief Closes the log file.
 */
void closeLogFile();

} // namespace log_internal
} // namespace rk

#endif // #ifndef LOG_H
