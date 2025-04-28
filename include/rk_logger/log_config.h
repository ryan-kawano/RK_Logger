/**
 * @file log_config.h
 * @brief Header file for the logging config.
 */
#ifndef LOG_CONFIG_H
#define LOG_CONFIG_H

#include <unordered_map>
#include <string>
#include <unordered_set>
#include <cstdint>
#include <filesystem>

namespace rk {
namespace config {

const inline std::string CONFIG_FILE_NAME = "rk_config.txt";

// Keys from the Config file
namespace configFileKeys {
    const std::string DATE_FORMAT = "date_format";
    const std::string MONTH_FORMAT = "month_format";
    const std::string TIME_FORMAT = "time_format";
    const std::string WRITE_TO_LOG_FILE = "write_to_log_file";
};

using ConfigKey = std::string;
using ConfigValue = int;
using PossibleValuesMap = const std::unordered_map<ConfigKey, ConfigValue>;
extern PossibleValuesMap dateFormatPossibleValues;
extern PossibleValuesMap monthFormatPossibleValues;
extern PossibleValuesMap timeFormatPossibleValues;
extern PossibleValuesMap writeToLogFilePossibleValues;

using ConfigMap = std::unordered_map<std::string, std::tuple<PossibleValuesMap, ConfigValue>>;
extern ConfigMap configuration; /**< Holds the current configuration */

/**
 * @brief Parses the logging config file and updates the settings based on the contents.
 * 
 * If no config file is provided, then it will just use the default settings.
 * 
 * @param path The path to the config file including the file name itself. The default path is the same directory as the executable.
 * Override it by providing a custom path.
 */
void getLoggingConfig(const std::filesystem::path&);

/**
 * @brief Updates the config map with the values provided.
 * 
 * @param ConfigMap::iterator An iterator to the key to be set.
 * @param std::string The value to set.
 */
void updateConfigValue(const ConfigMap::iterator&, const ConfigValue);

} // namespace config
} // namespace rk

#endif // #ifndef LOG_CONFIG_H
