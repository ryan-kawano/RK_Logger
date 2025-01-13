/**
 * @file log_time.h
 * @brief Header file for code related to time for logs.
 */
#ifndef LOG_TIME_H
#define LOG_TIME_H

#include <chrono>
#include <string>

namespace rk {
namespace time {

    typedef std::chrono::system_clock system_clock;
    typedef std::chrono::system_clock::time_point time_point;

    constexpr const char* months[12] = {
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec"
    };

    /**
     * @brief Generates a time-stamp from a time_point.
     * 
     * @param time_point The time_point to convert.
     */
    std::string generateTimeStamp(time_point);

    /**
     * @brief Converts a month number to the english name.
     * 
     * It starts at 0 (not 1) and goes up, so January = 0, February = 1, etc.
     * 
     * @brief The month number.
     * @return The month's name abbreviated to the first 3 letters, e.g., Jan, Feb, etc.
     */
    std::string monthNumToName(const uint8_t);

} // namespace time
} // namespace rk

#endif // #ifndef LOG_TIME_H
