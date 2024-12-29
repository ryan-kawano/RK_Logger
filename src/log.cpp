/**
 * @file log.cpp
 * @brief The implementation file for the logger.
 */
#include "logger/log.h"

namespace rk {
namespace log {

/**
 * This function will continously check the log queue for new messages and
 * print them via std::cout. It will end the loop once the endLoop flag is set
 * to true and the queue is empty.
 */
void logQueueLoop() {
    std::string msg;
    while (true) {
        std::lock_guard<std::mutex> logLock(logQueueMutex);
        if (!logQueue.empty()) {
            msg = logQueue.front();
            logQueue.pop();
            if (!msg.empty()){
                std::cout << msg;
                logFile << msg;
                msg.clear();
            }
        }

        std::lock_guard<std::mutex> endLoopLock(endLoopMtx);
        if (endLoop && logQueue.empty()) {
            break;
        }
    }
}

/**
 * Creates the thread that will run the log loop function - logQueueLoop.
 */
std::thread startLogThread() {
    std::thread logThread(logQueueLoop);
    return logThread;
}

/**
 * Ends the log thread that was passed in by first setting the endLoop flag to
 * true. This will allow the log loop to exit. Then, it will join the thread.
 */
void endLogThread(std::thread& thread) {
    {
        std::lock_guard<std::mutex> lock(endLoopMtx);
        endLoop = true;
    }

    if (thread.joinable()) {
        thread.join();
    }
}

/**
 * Closes the log file by using std::ofstream::close().
 */
void closeLogFile() {
    logFile.close();
}

} // namespace log
} // namespace rk
