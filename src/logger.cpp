/**
 * @file log.cpp
 * @brief Source file for the logger.
 */
#include "rk_logger/logger.h"

namespace rk {
namespace log {

std::mutex logQueueMutex;
std::queue<std::string> logQueue;
std::mutex endLoopMtx;
bool endLogLoop;
std::ofstream logFile;
std::condition_variable cv;

std::thread startLogger(const std::filesystem::path& configPath) {
    // Read config settings from a file (if it exists) and update the internal config
    rk::config::getLoggingConfig(configPath);
    rk::time::updateTimeStampFuncs();

    std::string timeStamp = rk::time::generateTimeStamp(rk::time::system_clock::now());
    timeStamp = rk::time::convertTimeStampForFileName(timeStamp);
    const std::string logFileName = std::string("logs_") + timeStamp + ".txt";
    std::cout << "Writing to log file: " << logFileName << std::endl;
    logFile.open(logFileName);

    std::thread logThread = startLogThread();
    LOG_VERIFY

    return logThread;
}

void stopLogger(std::thread logThread) {
    endLogThread(std::move(logThread));
    closeLogFile();
}

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
                logFile << msg;
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

void closeLogFile() {
    logFile.close();
}

} // namespace log
} // namespace rk
