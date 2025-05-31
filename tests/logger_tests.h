#ifndef LOGGER_TESTS_H
#define LOGGER_TESTS_H

#include <rk_logger_tests/test_base.h>

class StartAndStopLoggerTest : public rk_logger_tests::Base {};

class LogMessageTest : public rk_logger_tests::Base, public ::testing::WithParamInterface<std::string> {
    void SetUp() override {
        redirectStdCout();
        ASSERT_NO_FATAL_FAILURE(Base::startLogger());
    }
    
    void TearDown() override {
        undoRedirectStdCout();
        ASSERT_NO_FATAL_FAILURE(Base::stopLogger());
    }
};

#endif // #ifndef LOGGER_TESTS_H