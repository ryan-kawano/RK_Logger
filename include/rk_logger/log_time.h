/**
 * @file log_time.h
 * @brief Header file for code related to time for logs.
 */
#ifndef LOG_TIME_H
#define LOG_TIME_H

#include <chrono>
#include <string>
#include <functional>
#include <mutex>

namespace rk {
namespace time {

typedef std::chrono::system_clock system_clock;
typedef std::chrono::system_clock::time_point time_point;

extern std::mutex tmMutex; /**< tm meaning std::tm */

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
 * @brief Adds zeros to the beginning of a number to make it have a specific number of characters.
 * 
 * This is meant for keeping the timestamps uniform in size.
 * 
 * @param std::string The number to modify.
 * @param size_t The target size to change it to.
 */
void padWithZeros(std::string&, const size_t);

/**
 * @brief Converts a timestamp in order to use it in a file name.
 * 
 * It will remove any characters that are not allowed in a file name and format it for readability. Meant to be used with the output from the timestamp function.
 * 
 * @param std::string The timestamp to convert.
 * @return The converted timestamp.
 */
std::string convertTimeStampForFileName(const std::string);

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
 * @brief Generates a formatted time that is specified by the config, with the given sub-units.
 * 
 * This function is determined at runtime. The sub-units should be in the same format as std::tm, i.e., hour is 0-23 hours since midnight, etc.
 * 
 * @param std::string The hour.
 * @param std::string The minute.
 * @param std::string The second.
 * @param std::string The millisecond.
 * @return The formatted time.
 */
extern std::function<std::string(std::string, const std::string, const std::string, const std::string)> timeFunc;

/**
 * @brief Updates the month function. This function does not "update the month", but rather updates the function that formats the month.
 */
void updateMonthFunc();

/**
 * @brief Updates the date function. This function itself does not "update the date", but rather updates the function that formats the date.
 */
void updateDateFunc();

/**
 * @brief Updates the time function. This function itself does not "update the time", but rather updates the function that formats the time.
 */
void updateTimeFunc();

/**
 * @brief Calls all the function updater functions.
 */
void updateTimeStampFuncs();

} // namespace time
} // namespace rk

#endif // #ifndef LOG_TIME_H
