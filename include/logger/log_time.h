/**
 * @file log_time.h
 * @brief Header file for code related to time for logs.
 */
#ifndef LOG_TIME_H
#define LOG_TIME_H

#include <chrono>
#include <string>
#include <functional>

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
 * It starts at 1 and goes up, so January = 1, February = 2, etc.
 * 
 * @brief The month number.
 * @return The month's name abbreviated to the first 3 letters, e.g., Jan, Feb, etc.
 */
std::string monthNumToName(const uint8_t);

/**
 * @brief Converts a month to the format specified by the config.
 * 
 * This function is determined at runtime.
 * 
 * @param int The month as a number starting at 1 and going up, so January = 1, February = 2, etc.
 * @return The formatted month.
 */
extern std::function<std::string(const int)> monthFunc;

/**
 * @brief Converts a date to the format specified by the config.
 * 
 * This function is determined at runtime.
 * 
 * @param std::string The year.
 * @param std::string The month.
 * @param std::string The day.
 * @return The formatted date.
 */
extern std::function<std::string(const std::string, const std::string, const std::string)> dateFunc;

/**
 * @brief Updates the month function at runtime.
 */
void updateMonthFunc();

/**
 * @brief Updates the date function at runtime.
 */
void updateDateFunc();

/**
 * @brief Calls all function updater functions to update functions at runtime.
 */
void updateTimeStampFuncs();

} // namespace time
} // namespace rk

#endif // #ifndef LOG_TIME_H
