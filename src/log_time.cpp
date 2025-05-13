/**
 * @file log_time.cpp
 * @brief Source file for code related to time for logs.
 */
#define RK_TIME_LOG(...) std::cout << "[RKLogger Time] " << __VA_ARGS__

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
    if (rk::config::getInstance().getConfigValueByKey(rk::config::month_format::KEY) == rk::config::month_format::MONTH_NUM) {
        padWithZeros(month, 2);
    }

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

std::string monthNumToName(const int monthNum) {
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

std::string convertTimeStampForFileName(std::string timeStamp) {
    for (auto iter = timeStamp.begin(); iter != timeStamp.end(); ) {
        if (*iter == '[' || *iter == ']') { // Remove any square brackets
            iter = timeStamp.erase(iter);
        }
        else if (*iter == '|') { // Replace any vertical lines with underscores
            *iter = '_';
            iter++;
        }
        else if (*iter == ':') { // Replace any colons with dashes
            *iter = '-';
            iter++;
        }
        else if (*iter == ' ') { // Remove any white spaces
            iter = timeStamp.erase(iter);
        }
        else {
            iter++;
        }
    }

    return timeStamp;
}

std::function<std::string(const int)> monthFunc = [] (const int monthNum) {
    std::string month;
    month = std::to_string(monthNum);
    return month;
};

std::function<std::string(const std::string, const std::string, const std::string)> dateFunc = [](const std::string year, const std::string month, const std::string day) {
    std::string date;
    date = "[" +
        month +
        "-" +
        day +
        "-" +
        year +
        "|";
    return date;
};

std::function<std::string(std::string, const std::string, const std::string, const std::string)> timeFunc = [] (std::string hour, const std::string minute, const std::string second, const std::string millisecond) {
    int hourNum = std::stoi(hour);
    const bool isPM = (hourNum >= 12) ? true : false;

    if (isPM && hourNum != 12) {
        hour = std::to_string(hourNum - 12);
        padWithZeros(hour, 2);
    }

    std::string time = hour + ":" + minute + ":" + second + "." + millisecond;
    if (isPM) {
        time += " PM";
    }
    else {
        time += " AM";
    }
    time += "]";

    return time;
};

void updateMonthFunc() {
    RK_TIME_LOG("Updating month function\n");
    const rk::config::ConfigValue monthFormat = rk::config::getInstance().getConfigValueByKey(rk::config::month_format::KEY);
    if (monthFormat == rk::config::month_format::MONTH_NUM) {
        monthFunc = [] (const int monthNum) {
            std::string month;
            month = std::to_string(monthNum);
            return month;
        };
    }
    else if(monthFormat == rk::config::month_format::MONTH_NAME) {
        monthFunc = [] (const int monthNum) {
            std::string month;
            month = monthNumToName(monthNum);
            return month;
        };
    }
}

void updateDateFunc() {
    RK_TIME_LOG("Updating date function\n");
    const rk::config::ConfigValue dateFormat = rk::config::getInstance().getConfigValueByKey(rk::config::date_format::KEY);
    if (dateFormat == rk::config::date_format::MM_DD_YYYY) {
        dateFunc = [](const std::string year, const std::string month, const std::string day) {
            std::string date;
            date = "[" +
                        month +
                        "-" +
                        day +
                        "-" +
                        year +
                        "|";
            return date;
        };
    }
    else if (dateFormat == rk::config::date_format::DD_MM_YYYY) {
        dateFunc = [](const std::string year, const std::string month, const std::string day) {
            std::string date;
            date = "[" +
                        day +
                        "-" +
                        month +
                        "-" +
                        year +
                        "|";
            return date;
        };
    }
    else if (dateFormat == rk::config::date_format::YYYY_MM_DD) {
        dateFunc = [](const std::string year, const std::string month, const std::string day) {
            std::string date;
            date = "[" +
                        year +
                        "-" +
                        month +
                        "-" +
                        day +
                        "|";
            return date;
        };
    }
}

void updateTimeFunc() {
    RK_TIME_LOG("Updating time function\n");
    const rk::config::ConfigValue hourFormat = rk::config::getInstance().getConfigValueByKey(rk::config::hour_format::KEY);
    if (hourFormat == rk::config::hour_format::TWELVE_HOUR) {
        timeFunc = [] (std::string hour, const std::string minute, const std::string second, const std::string millisecond) {
            int hourNum = std::stoi(hour);
            const bool isPM = (hourNum >= 12) ? true : false;

            if (isPM && hourNum != 12) {
                hour = std::to_string(hourNum - 12);
                padWithZeros(hour, 2);
            }

            std::string time = hour + ":" + minute + ":" + second + "." + millisecond;
            if (isPM) {
                time += " PM";
            }
            else {
                time += " AM";
            }
            time += "]";

            return time;
        };
    }
    else if(hourFormat == rk::config::hour_format::TWENTY_FOUR_HOUR) {
        timeFunc = [] (std::string hour, const std::string minute, const std::string second, const std::string millisecond) {
            return hour + ":" + minute + ":" + second + "." + millisecond + "]";
        };
    }
}

void updateTimeStampFuncs() {
    RK_TIME_LOG("Updating timestamp functions\n");
    updateMonthFunc();
    updateDateFunc();
    updateTimeFunc();
}

} // namespace time
} // namespace rk
