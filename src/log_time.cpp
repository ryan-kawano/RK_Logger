/**
 * @file log_time.cpp
 * @brief Source file for code related to time for logs.
 */
#include <iomanip>

#include "logger/log_time.h"

namespace rk {
namespace time {

    std::string generateTimeStamp(time_point time_point) {
        // Convert to other type get the parts of the time
        std::time_t time_t = system_clock::to_time_t(time_point);
        std::tm tm_local = *std::localtime(&time_t);

        std::string year = std::to_string(tm_local.tm_year + 1900); // tm_year starts from 1900
        std::string day = std::to_string(tm_local.tm_mday);
        day = (tm_local.tm_mday < 10 ? "0" : "") + day; // Change any single-digit days to double-digit by adding a 0
        std::string hour = std::to_string(tm_local.tm_hour);
        std::string minute = std::to_string(tm_local.tm_min);
        std::string second = std::to_string(tm_local.tm_sec);

        // Get month based on the setting specified (as a name or a number)
        std::string month;
        if (MONTH_FORMAT & USE_MONTH_NUM) {
            month = std::to_string(tm_local.tm_mon + 1);
            month = (tm_local.tm_mon < 9 ? "0" : "") + month; // Add a 0 to the beginning of any single-digit months to make it 2 digits. tm_mon starts at 0, so 9 is max, not 10.
        }
        else if(MONTH_FORMAT & USE_MONTH_NAME) {
            month = monthNumToName(tm_local.tm_mon);
        }
        else { // Generic form if MONTH_FORMAT isn't specified. This is just in case MONTH_FORMAT gets undefined, it should always be defined though.
            month = std::to_string(tm_local.tm_mon + 1);
            month = (tm_local.tm_mon < 9 ? "0" : "") + month; // Add a 0 to the beginning of any single-digit months to make it 2 digits. tm_mon starts at 0, so 9 is max, not 10.
        }

        // Need to calculate milliseconds from nanoseconds
        auto seconds_time_point = std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch());
        std::string milliseconds = std::to_string((time_point.time_since_epoch().count() - std::chrono::duration_cast<std::chrono::nanoseconds>(seconds_time_point).count()) / static_cast<int>(1e6));
        
        // Format date based on the setting provided
        std::string timeStamp;
        if (DATE_FORMAT & USE_MM_DD_YYYY) {
            timeStamp = "[" +
                        month +
                        "-" +
                        day +
                        "-" +
                        year +
                        "|";
        }
        else if (DATE_FORMAT & USE_DD_MM_YYYY) {
            timeStamp = "[" +
                        day +
                        "-" +
                        month +
                        "-" +
                        year +
                        "|";
        }
        else if (DATE_FORMAT & USE_YYYY_MM_DD) {
            timeStamp = "[" +
                        year +
                        "-" +
                        month +
                        "-" +
                        day +
                        "|";
        }
        else { // Generic form if DATE_FORMAT isn't specified. This is just in case DATE_FORMAT gets undefined, it should always be defined though.
            timeStamp = "[" +
                        year +
                        "-" +
                        month +
                        "-" +
                        day +
                        "|";
        }

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
        if (monthNum < 0 || monthNum > 12) {
            return "N/A";
        }

        return months[monthNum];
    }

} // namespace time
} // namespace rk
