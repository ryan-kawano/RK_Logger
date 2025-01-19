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

    using PossibleValuesMap = const std::unordered_map<std::string, uint8_t>;
    extern PossibleValuesMap dateFormatPossibleValues;
    extern PossibleValuesMap monthFormatPossibleValues;

    using ActualValue = uint8_t; /**< The type of value that will be used for the Config setting */
    using ConfigMap = std::unordered_map<std::string, std::tuple<PossibleValuesMap, ActualValue>>;
    extern ConfigMap configMap; /**< Holds the current configuration along with other config-related info */

    /**
     * @brief Parses the logging config file and updates the settings based on the contents.
     * 
     * If no config file is provided, then it will just use the default settings.
     * 
     * @param path The path to the config file including the file name itself. The default path is the same directory as the executables.
     * Override it by providing a custom path.
     */
    void getLoggingConfig(const std::filesystem::path& path = std::filesystem::current_path()/rk::config::CONFIG_FILE_NAME);

    /**
     * @brief Updates the config map with the values provided.
     * 
     * @param ConfigMap::iterator An iterator to the key to be set.
     * @param std::string The value to set.
     */
    void updateConfigValue(const ConfigMap::iterator&, const ActualValue);

} // namespace config
} // namespace rk

#endif // #ifndef LOG_CONFIG_H
