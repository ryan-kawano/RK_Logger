/**
 * @file log.cpp
 * @brief Source file for the logger.
 */
#include <rk_logger/logger.h>
#include <rk_logger/log_time.h>

namespace rk {
namespace log {

std::thread startLogger(const std::filesystem::path& configPath) {
    rk::log_internal::rkLogInternal("Starting RK Logger\n");

    // Read config settings from a file (if it exists) and update the internal config
    rk::config::getInstance().parseLoggingConfig(configPath);
    rk::time_internal::updateTimeStampFuncs();

    if (rk::config::getInstance().getConfigValueByKey(rk::config::write_to_log_file::KEY) == rk::config::write_to_log_file::ENABLE) {
        rk::log_internal::openLogFile();
        rk::log_internal::verifyLogFile();
    }

    LOG_VERIFY
    std::thread logThread = rk::log_internal::startLogThread();

    return logThread;
}

void stopLogger(std::thread logThread) {
    rk::log_internal::rkLogInternal("Stopping RK Logger\n");
    rk::log_internal::endLogThread(std::move(logThread));
    if (rk::config::getInstance().getConfigValueByKey(rk::config::write_to_log_file::KEY) == rk::config::write_to_log_file::ENABLE) {
        rk::log_internal::closeLogFile();
    }
}

} // namespace log
} // namespace rk

namespace rk {
namespace log_internal {

std::mutex logQueueMutex;
std::queue<std::string> logQueue;
std::mutex endLoopMtx;
bool endLogLoop;
std::ofstream logFile;
std::condition_variable logQueueCv;

/**
 * Checks whether the log queue has messages or the endLogLoop flag is true. If either are true, it will return true,
 * stop the condition variable from waiting, and resume execution of the log loop. This should be called as part of
 * the log loop's condition variable, so the mutex for the log queue will be locked by the condition variable when
 * this function executes, so it doesn't need to be locked explicitly.
 */
bool condVarPredicate() {
    std::unique_lock<std::mutex> endLoopLock(endLoopMtx);
    return !logQueue.empty() || endLogLoop;
}

/**
 * This function will continously check the log queue for new messages and print them via std::cout.
 * If no logs are in the queue, it'll wait until new ones get added.
 * It will end the loop once the endLogLoop flag is set to true and the queue is empty.
 */
void logQueueLoop() {
    std::string msg;
    while (true) {
        std::unique_lock<std::mutex> logLock(logQueueMutex);
        if (!logQueue.empty()) {
            msg = logQueue.front();
            logQueue.pop();
            if (!msg.empty()){
                std::cout << msg;
                if (logFile) {
                    logFile << msg;
                }
                msg.clear();
            }
        }
        else {
            std::unique_lock<std::mutex> endLoopLock(endLoopMtx);
            if (!endLogLoop) {
                endLoopLock.unlock();
                cv.wait(logLock, rk::log::condVarPredicate);
            }
            else {
                break;
            }
        }
    }
}

std::thread startLogThread() {
    std::thread logThread(logQueueLoop);
    return logThread;
}

/**
 * Ends the log thread that was passed in by first setting the endLogLoop flag to
 * true. This will allow the log loop to exit. Then, it will join the thread.
 */
void endLogThread(std::thread thread) {
    {
        std::lock_guard<std::mutex> lock(endLoopMtx);
        endLogLoop = true;
        cv.notify_all();
    }

    if (thread.joinable()) {
        thread.join();
    }
}

void openLogFile() {
    std::string timeStamp = rk::time_internal::generateTimeStamp(rk::time_internal::system_clock::now());
    timeStamp = rk::time_internal::convertTimeStampForFileName(timeStamp);
    const std::string logFileName = std::string("logs_") + timeStamp + ".txt";
    rkLogInternal("Writing to log file: ", logFileName, "\n");
    logFile.open(logFileName);
}

void closeLogFile() {
    logFile.close();
}

} // namespace log_internal
} // namespace rk
