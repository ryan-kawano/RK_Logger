/**
 * @file log_time.h
 * @brief Header file for code related to time for logs.
 */
#ifndef LOG_TIME_H
#define LOG_TIME_H

// Bit Mask that determines what format to print the date in
#define DATE_FORMAT 0x0 | USE_YYYY_MM_DD
#define USE_MM_DD_YYYY 0x1 // i.e., Feb 4, 2025 is formatted as 02-04-2025
#define USE_DD_MM_YYYY 0x2 // i.e., Feb 4, 2025 is formatted as 04-02-2025
#define USE_YYYY_MM_DD 0x4 // i.e., Feb 4, 2025 is formatted as 2025-02-04

// Bit Mask that determines whether to print the name of the month or the month number
#define MONTH_FORMAT 0x0 | USE_MONTH_NUM
#define USE_MONTH_NUM 0x1 // Prints the months name, e.g., Jan, Feb, etc.
#define USE_MONTH_NAME 0x2 // Prints the month's number, e.g., 01, 02, etc.

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
     * @brief Converts a number to the english name.
     * 
     * It starts at 0 and goes up, so January = 0, February = 1, etc.
     * 
     * @brief The month number.
     * @return The month's name abbreviated to the first 3 letters.
     */
    std::string monthNumToName(const uint8_t);

} // namespace time
} // namespace rk

#endif // #ifndef LOG_TIME_H
