/**
 * @file log_time.cpp
 * @brief Source file for code related to time for logs.
 */
#include <iomanip>
#include <iostream>

#include "rk_logger/log_time.h"
#include "rk_logger/log_config.h"

namespace rk {
namespace time {

std::mutex tmMutex;

std::string generateTimeStamp(time_point time_point) {
    // Convert time to other type for easier access to sub-units
    std::time_t time_t = system_clock::to_time_t(time_point);
    std::tm tm_local;
    {
        std::lock_guard<std::mutex> lock(tmMutex);
        tm_local = *std::localtime(&time_t);
    }

    std::string year = std::to_string(tm_local.tm_year + 1900); // tm_year starts from 1900
    std::string day = std::to_string(tm_local.tm_mday);
    padWithZeros(day, 2);
    std::string hour = std::to_string(tm_local.tm_hour);
    padWithZeros(hour, 2);
    std::string minute = std::to_string(tm_local.tm_min);
    padWithZeros(minute, 2);
    std::string second = std::to_string(tm_local.tm_sec);
    padWithZeros(second, 2);

    // Get month based on the config settings (as a name or as a number)
    std::string month;
    month = monthFunc(tm_local.tm_mon + 1); // Add 1 because std::tm's months start at 0
    padWithZeros(month, 2);

    /**
     * Need to calculate milliseconds. Convert the current time to seconds in order to lose precision and lose
     * the fractional portion. Then, subtract that number from the current time in milliseconds and the difference
     * should be the milliseconds portion of the current time.
     */
    auto time_point_as_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch()).count();
    auto time_point_as_s = std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count();
    auto diff = time_point_as_ms - (time_point_as_s * static_cast<uint64_t>(1e3)); // Need to multiply seconds by 1e3 in order to get milliseconds
    std::string milliseconds = std::to_string(diff);
    padWithZeros(milliseconds, 3);

    // Format date and time based on the config settings
    std::string date = dateFunc(year, month, day);
    std::string time = timeFunc(hour, minute, second, milliseconds);

    const std::string timeStamp = date + time;

    return timeStamp;
}

std::string monthNumToName(const uint8_t monthNum) {
    if (monthNum < 1 || monthNum > 12) {
        return "N/A";
    }

    return months[monthNum - 1]; // Subtract one because the index of months array starts at 0
}

void padWithZeros(std::string& number, const int targetSize) {
    // If target size is 1, then no zeros need to be added, so just return. Same for empty numbers.
    if (number.empty() || targetSize < 2) {
        return;
    }
    if (number.size() == targetSize) {
        return;
    }

    const size_t amountOfZeros = targetSize - number.size();
    const std::string zeros(amountOfZeros, '0');
    number = zeros + number;
}

std::string convertTimeStampForFileName(const std::string timeStamp) {
    std::string timeStampForFileName = timeStamp;

    // Remove any square brackets
    size_t squareBracketIdx = timeStampForFileName.find_first_of("[]");
    while (squareBracketIdx != std::string::npos) {
        timeStampForFileName.replace(squareBracketIdx, 1, "");
        squareBracketIdx = timeStampForFileName.find_first_of("[]");
    }

    // Replace any vertical lines with underscores
    size_t verticalLineIdx = timeStampForFileName.find_first_of("|");
    while (verticalLineIdx != std::string::npos) {
        timeStampForFileName.replace(verticalLineIdx, 1, "_");
        verticalLineIdx = timeStampForFileName.find_first_of("|");
    }

    // Replace any colons with dashes
    size_t colonIndex = timeStampForFileName.find_first_of(":");
    while (colonIndex != std::string::npos) {
        timeStampForFileName.replace(colonIndex, 1, "-");
        colonIndex = timeStampForFileName.find_first_of(":");
    }

    return timeStampForFileName;
}

std::function<std::string(const int)> monthFunc = nullptr;

std::function<std::string(const std::string, const std::string, const std::string)> dateFunc = nullptr;

std::function<std::string(std::string, const std::string, const std::string, const std::string)> timeFunc = nullptr;

void updateMonthFunc() {
    std::cout << "Updating month function\n";
    const rk::config::ActualValue monthFormat = std::get<1>(rk::config::configuration.at("month_format"));
    if (monthFormat == std::get<0>(rk::config::configuration.at("month_format")).at("MONTH_NUM")) {
        monthFunc = [] (const int monthNum) {
            std::string month;
            month = std::to_string(monthNum);
            return month;
        };
    }
    else if(monthFormat == std::get<0>(rk::config::configuration.at("month_format")).at("MONTH_NAME")) {
        monthFunc = [] (const int monthNum) {
            std::string month;
            month = monthNumToName(monthNum);
            return month;
        };
    }
}

void updateDateFunc() {
    std::cout << "Updating date function\n";
    const rk::config::ActualValue dateFormat = std::get<1>(rk::config::configuration.at("date_format"));
    if (dateFormat == std::get<0>(rk::config::configuration.at("date_format")).at("MM_DD_YYYY")) {
        dateFunc = [](const std::string year, const std::string month, const std::string day) {
            std::string timeStamp;
            timeStamp = "[" +
                        month +
                        "-" +
                        day +
                        "-" +
                        year +
                        "|";
            return timeStamp;
        };
    }
    else if (dateFormat == std::get<0>(rk::config::configuration.at("date_format")).at("DD_MM_YYYY")) {
        dateFunc = [](const std::string year, const std::string month, const std::string day) {
            std::string timeStamp;
            timeStamp = "[" +
                        day +
                        "-" +
                        month +
                        "-" +
                        year +
                        "|";
            return timeStamp;
        };
    }
    else if (dateFormat == std::get<0>(rk::config::configuration.at("date_format")).at("YYYY_MM_DD")) {
        dateFunc = [](const std::string year, const std::string month, const std::string day) {
            std::string timeStamp;
            timeStamp = "[" +
                        year +
                        "-" +
                        month +
                        "-" +
                        day +
                        "|";
            return timeStamp;
        };
    }
}

void updateTimeFunc() {
    std::cout << "Updating time function\n";
    const rk::config::ActualValue timeFormat = std::get<1>(rk::config::configuration.at("time_format"));
    if (timeFormat == std::get<0>(rk::config::configuration.at("time_format")).at("12")) {
        timeFunc = [] (std::string hour, const std::string minute, const std::string second, const std::string millisecond) {
            int hourNum = std::stoi(hour);
            const bool isPM = (hourNum >= 12) ? true : false;

            if (isPM && hourNum != 12) {
                hour = std::to_string(hourNum - 12);
                padWithZeros(hour, 2);
            }

            std::string time = hour + ":" + minute + ":" + second + "." + millisecond;
            if (isPM) {
                time = time + " PM";
            }
            else {
                time = time + " AM";
            }
            time = time + "]";

            return time;
        };
    }
    else if(timeFormat == std::get<0>(rk::config::configuration.at("time_format")).at("24")) {
        timeFunc = [] (std::string hour, const std::string minute, const std::string second, const std::string millisecond) {
            return hour + ":" + minute + ":" + second + "." + millisecond + "]";
        };
    }
}

void updateTimeStampFuncs() {
    std::cout << "Updating timestamp functions\n";
    updateMonthFunc();
    updateDateFunc();
    updateTimeFunc();
}

} // namespace time
} // namespace rk
