/**
 * @file log_time.cpp
 * @brief Source file for code related to time for logs.
 */
#include <iomanip>

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

    // Need to calculate milliseconds from nanoseconds
    auto seconds_time_point = std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch());
    std::string milliseconds = std::to_string((time_point.time_since_epoch().count() - std::chrono::duration_cast<std::chrono::nanoseconds>(seconds_time_point).count()) / static_cast<int>(1e6));
    padWithZeros(milliseconds, 3);

    // Format date based on the config settings
    std::string timeStamp = dateFunc(year, month, day);

    timeStamp = timeStamp + hour +
                            ":" +
                            minute +
                            ":" +
                            second +
                            "." +
                            milliseconds +
                            "]";
    return timeStamp;
}

std::string monthNumToName(const uint8_t monthNum) {
    if (monthNum < 1 || monthNum > 12) {
        return "N/A";
    }

    return months[monthNum - 1]; // Subtract one because the index of months array starts at 0
}

void padWithZeros(std::string& number, const size_t targetSize) {
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

void updateMonthFunc() {
    const rk::config::ActualValue monthFormat = std::get<1>(rk::config::configMap.at("month_format"));
    if (monthFormat == std::get<0>(rk::config::configMap.at("month_format")).at("MONTH_NUM")) {
        monthFunc = [] (const int monthNum) {
            std::string month;
            month = std::to_string(monthNum);
            return month;
        };
    }
    else if(monthFormat == std::get<0>(rk::config::configMap.at("month_format")).at("MONTH_NAME")) {
        monthFunc = [] (const int monthNum) {
            std::string month;
            month = monthNumToName(monthNum);
            return month;
        };
    }
}

void updateDateFunc() {
    const rk::config::ActualValue dateFormat = std::get<1>(rk::config::configMap.at("date_format"));
    if (dateFormat == std::get<0>(rk::config::configMap.at("date_format")).at("MM_DD_YYYY")) {
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
    else if (dateFormat == std::get<0>(rk::config::configMap.at("date_format")).at("DD_MM_YYYY")) {
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
    else if (dateFormat == std::get<0>(rk::config::configMap.at("date_format")).at("YYYY_MM_DD")) {
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

void updateTimeStampFuncs() {
    updateMonthFunc();
    updateDateFunc();
}

} // namespace time
} // namespace rk
