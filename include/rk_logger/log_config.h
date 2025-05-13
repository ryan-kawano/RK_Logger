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

using ConfigKey = std::string;
using ConfigValue = std::string;
using ConfigMap = std::unordered_map<ConfigKey, ConfigValue>;
using ValidValuesSet = std::unordered_set<ConfigValue>;
using ValidKeyValuesMap = const std::unordered_map<ConfigKey, const ValidValuesSet>;

extern const std::string CONFIG_FILE_NAME;

namespace date_format {
    extern const std::string KEY;
    extern const std::string MM_DD_YYYY; // e.g., Feb 4, 2025 is formatted as [02|Feb]-04-2025
    extern const std::string DD_MM_YYYY; // e.g., Feb 4, 2025 is formatted as 04-[02|Feb]-2025
    extern const std::string YYYY_MM_DD; // e.g., Feb 4, 2025 is formatted as 2025-[02|Feb]-04
}

namespace month_format {
    extern const std::string KEY;
    extern const std::string MONTH_NUM; // Month as a name, e.g., Jan, Feb, etc.
    extern const std::string MONTH_NAME; // Month as a number, e.g., 01, 02, etc.
}

namespace hour_format {
    extern const std::string KEY;
    extern const std::string TWELVE_HOUR; // 12-hour clock format, e.g., 8:30PM becomes "08:30:00.000 PM"
    extern const std::string TWENTY_FOUR_HOUR; // 24-hour clock format, e.g., 8:30PM becomes "20:30:00.000"
}

namespace write_to_log_file {
    extern const std::string KEY;
    extern const std::string ENABLE;
    extern const std::string DISABLE;
}

class Config {
public:
    Config() = delete;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    friend Config& getInstance();

    /**
     * @brief Sets a value for a key/value pair in the Config.
     * 
     * @param ConfigKey The key to change the value for.
     * @param ConfigValue The value to change it to.
     */
    void setConfigValue(const ConfigKey, const ConfigValue);

    /**
     * @brief Gets the current config's value for a given key.
     * 
     * @param ConfigKey The key.
     * @return The corresponding value.
     */
    ConfigValue getConfigValueByKey(const ConfigKey) const;

    /**
     * @brief Parses the logging config file and updates the settings based on the contents.
     * 
     * @param path The path to the config file including the file name itself. The default path is the same directory as the executable.
     */
    void parseLoggingConfig(const std::filesystem::path&);

    /**
     * @brief Checks if a key is valid.
     * 
     * @param ConfigKey The key to check.
     * @return True if key is valid, false otherwise.
     */
    bool isKeyValid(const ConfigKey) const;

    /**
     * @brief Checks if both a key and value are valid.
     * 
     * @param ConfigKey The key to check.
     * @param ConfigValue The corresponding value to check.
     * @return True if both key and value are valid, false otherwise.
     */
    bool isKeyAndValueValid(const ConfigKey, const ConfigValue) const;

private:
    Config(ConfigMap&& defaultConfig, ValidKeyValuesMap&& keyValues) : config(std::move(defaultConfig)), validKeyValues(std::move(keyValues)) {};

    ConfigMap config;
    ValidKeyValuesMap validKeyValues;
};

/**
 * @brief Gets the instance of the config class.
 * 
 * @return The instance.
 */
Config& getInstance();

} // namespace config
} // namespace rk

#endif // #ifndef LOG_CONFIG_H
