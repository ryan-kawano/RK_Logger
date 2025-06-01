#ifndef TEST_BASE_H
#define TEST_BASE_H

#include <gtest/gtest.h>
#include <rk_logger/logger.h>

namespace rk_logger_tests {

inline const std::chrono::milliseconds MAX_DELAY_FOR_ONE_MESSAGE = std::chrono::milliseconds(1);

class Base : public::testing::Test {
protected:
    void startLogger() {
        ASSERT_NO_THROW({
            logThread = rk::log::startLogger();
        });
        ASSERT_TRUE(logThread.joinable());
    }

    void stopLogger() {
        ASSERT_NO_THROW({
            rk::log::stopLogger(std::move(logThread));
        });
        ASSERT_FALSE(logThread.joinable());
    }

    void redirectStdCout() {
        coutBufOriginal = std::cout.rdbuf(logOutput.rdbuf());
    }

    void undoRedirectStdCout() const {
        std::cout.rdbuf(coutBufOriginal);
    }

    std::thread logThread;
    std::streambuf* coutBufOriginal;
    std::stringstream logOutput;
};

struct BaseParam {
    BaseParam(const std::string description) : description(description) {};
    virtual ~BaseParam() = default;

    const std::string description;
};

} // namespace rk_logger_tests

#endif // TEST_BASE_H
