#include <gtest/gtest.h>
#include <rk_logger/logger.h>
#include "logger_tests.h"

namespace rk_logger_tests {

TEST_F(StartAndStopLoggerTest, StartAndStopLogger) {
    SCOPED_TRACE("Starting the logger");
    ASSERT_NO_FATAL_FAILURE(Base::startLogger());

    SCOPED_TRACE("Stopping the logger");
    ASSERT_NO_FATAL_FAILURE(Base::stopLogger());
}

TEST_P(LogMessageTest, LogAMessage) {
    SCOPED_TRACE("Logging a message");
    RK_LOG(GetParam());

    SCOPED_TRACE("Waiting some time");
    std::this_thread::sleep_for(MAX_DELAY_FOR_ONE_MESSAGE);

    SCOPED_TRACE("Looking for the message in the logs");
    ASSERT_NE(logOutput.str().find(GetParam()), std::string::npos);
}

const std::vector<std::string> messages = {
    "Hello",
    "Hello\n",
    "\tHello",
    "\tHello\n",
    "Message with spaces",
    "123456789",
    "\n",
    "\t",
    "!",
    "@",
};
INSTANTIATE_TEST_SUITE_P(LogMessageTest, LogMessageTest, testing::ValuesIn(messages));

} // namespace rk_logger_tests
