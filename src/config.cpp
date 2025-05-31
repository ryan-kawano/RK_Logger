/**
 * @file log_config.cpp
 * @brief Source file for the logging config.
 */
#include <iostream>
#include <fstream>

#include <rk_logger/config.h>

namespace rk {
namespace config {


namespace date_format {
    const std::string KEY = "date_format";
    const std::string MM_DD_YYYY = "MM_DD_YYYY";
    const std::string DD_MM_YYYY = "DD_MM_YYYY";
    const std::string YYYY_MM_DD = "YYYY_MM_DD";
}

namespace month_format {
    const std::string KEY = "month_format";
    const std::string MONTH_NUM = "MONTH_NUM";
    const std::string MONTH_NAME = "MONTH_NAME";
}

namespace hour_format {
    const std::string KEY = "hour_format";
    const std::string TWELVE_HOUR = "12";
    const std::string TWENTY_FOUR_HOUR = "24";
}

namespace write_to_log_file {
    const std::string KEY = "write_to_log_file";
    const std::string DISABLE = "DISABLE";
    const std::string ENABLE = "ENABLE";
}

void Config::setConfigValue(const ConfigKey key, const ConfigValue val) {
    if (!isKeyAndValueValid(key, val)) {
        return;
    }
    config[key] = val;
}

ConfigValue Config::getConfigValueByKey(const ConfigKey key) const {
    if (!isKeyValid(key)) {
        return "";
    }
    return config.at(key);
}

/**
 * @brief Parses the logging config file and updates the settings based on the contents.
 * 
 * If no config file is provided, then it will just use the default settings.
 * 
 * @param path The path to the config file including the file name itself. The default path is the same directory as the executable.
 */
void Config::parseLoggingConfig(const std::filesystem::path& path) {
    rk::config_internal::cfgLog("Getting RK Logger config file", "\n");

    if (path.empty()) {
        rk::config_internal::cfgLog("The path to the config was empty. Using the default RK Logger config.", "\n");
        return;
    } 
    if (!std::filesystem::exists(path)) {
        rk::config_internal::cfgLog("A config file didn't exist at the provided path ", path, ". Using the default RK Logger config", "\n");
        return;
    }

    rk::config_internal::cfgLog("Trying to open RK Logger config file at path ", path, "", "\n");
    std::ifstream configFile(path);
    if (!configFile) {
        rk::config_internal::cfgLog("Could not open RK Logger config file. Either one wasn't provided or the path provided was invalid. Using default RK Logger config", "\n");
        return;
    }
    else {
        rk::config_internal::cfgLog("Successfully opened the RK Logger config file", "\n");
    }

    // Read through the file and update the config with any valid settings
    std::string line;
    while (std::getline(configFile, line)) {
        // Skip anything that's a comment or doesn't have an = sign
        const bool hasStar = line.find('*') != std::string::npos;
        const size_t equalsIndex = line.find_first_of('=');
        if (hasStar || equalsIndex == std::string::npos) {
            continue;
        }

        const ConfigKey configKey = line.substr(0, equalsIndex);
        if (!isKeyValid(configKey)) {
            rk::config_internal::cfgLog("Key \"", configKey, "\" was not valid. Not updating.", "\n");
            continue;
        }

        ConfigValue configValue = line.substr(equalsIndex + 1);
        // Remove new line at end of configValue, if it exists
        const size_t newLineIndex = configValue.find_last_of("\n");
        if (newLineIndex != std::string::npos && newLineIndex == configValue.size() - 1) {
            configValue = configValue.substr(0, configValue.size());
        }
        if (!isKeyAndValueValid(configKey, configValue)) {
            rk::config_internal::cfgLog("Value \"" , configValue, "\" for key \"", configKey, "\" was not valid. Not updating.", "\n");
            continue;
        }

        rk::config_internal::cfgLog("Updating config key \"", configKey, "\" with value \"", configValue, "\"", "\n");
        setConfigValue(configKey, configValue);
    }
}

bool Config::isKeyValid(const ConfigKey key) const {
    if (key.empty()) {
        return false;
    }
    return validKeyValues.find(key) != validKeyValues.end();
}

bool Config::isKeyAndValueValid(const ConfigKey key, const ConfigValue value) const {
    if (value.empty() || !isKeyValid(key)) {
        return false;
    }
    const ValidValuesSet& validValues = validKeyValues.at(key);
    return validValues.find(value) != validValues.end();
}

const ValidKeyValuesMap& Config::getValidKeyValues() const {
    return validKeyValues;
}

Config& getInstance() {
    static Config configuration(rk::config_internal::defaultConfig, rk::config_internal::validKeyValues);
    return configuration;
}

} // namespace config
} // namespace rk

namespace rk {
namespace config_internal {

const rk::config::ValidValuesSet dateFormat = {
    rk::config::date_format::MM_DD_YYYY,
    rk::config::date_format::DD_MM_YYYY,
    rk::config::date_format::YYYY_MM_DD,
};

const rk::config::ValidValuesSet monthFormat = {
    rk::config::month_format::MONTH_NUM,
    rk::config::month_format::MONTH_NAME,
};

const rk::config::ValidValuesSet hourFormat = {
    rk::config::hour_format::TWELVE_HOUR,
    rk::config::hour_format::TWENTY_FOUR_HOUR,
};

const rk::config::ValidValuesSet writeToLogFile = {
    rk::config::write_to_log_file::DISABLE,
    rk::config::write_to_log_file::ENABLE,
};

const rk::config::ValidKeyValuesMap validKeyValues = {
    { rk::config::date_format::KEY, dateFormat },
    { rk::config::month_format::KEY, monthFormat },
    { rk::config::hour_format::KEY, hourFormat },
    { rk::config::write_to_log_file::KEY, writeToLogFile },
};

const rk::config::ConfigMap defaultConfig = {
    { rk::config::date_format::KEY, rk::config::date_format::MM_DD_YYYY },
    { rk::config::month_format::KEY, rk::config::month_format::MONTH_NUM },
    { rk::config::hour_format::KEY, rk::config::hour_format::TWELVE_HOUR },
    { rk::config::write_to_log_file::KEY, rk::config::write_to_log_file::ENABLE },
};

} // namespace config_internal
} // namespace rk
