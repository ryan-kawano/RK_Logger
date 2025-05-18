/**
 * @file log_config.cpp
 * @brief Source file for the logging config.
 */
#define RK_CFG_LOG(...) std::cout << "[RKLogger Config] " << __VA_ARGS__
#include <iostream>
#include <fstream>

#include <rk_logger/config.h>

namespace rk {
namespace config {

const std::string CONFIG_FILE_NAME = "rk_config.txt";

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

// The data in this namespace is just the key/value constants above put into containers to allow querying the data.
namespace {
    ValidValuesSet dateFormat = {
        date_format::MM_DD_YYYY,
        date_format::DD_MM_YYYY,
        date_format::YYYY_MM_DD,
    };

    ValidValuesSet monthFormat = {
        month_format::MONTH_NUM,
        month_format::MONTH_NAME,
    };

    ValidValuesSet hourFormat = {
        hour_format::TWELVE_HOUR,
        hour_format::TWENTY_FOUR_HOUR,
    };

    ValidValuesSet writeToLogFile = {
        write_to_log_file::DISABLE,
        write_to_log_file::ENABLE,
    };

    ValidKeyValuesMap validKeyValues = {
        { date_format::KEY, dateFormat },
        { month_format::KEY, monthFormat },
        { hour_format::KEY, hourFormat },
        { write_to_log_file::KEY, writeToLogFile },
    };

    ConfigMap defaultConfig = {
        { date_format::KEY, date_format::MM_DD_YYYY },
        { month_format::KEY, month_format::MONTH_NUM },
        { hour_format::KEY, hour_format::TWELVE_HOUR },
        { write_to_log_file::KEY, write_to_log_file::ENABLE },
    };
} // namespace


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
    RK_CFG_LOG("Getting RK Logger config file" << std::endl);

    if (path.empty()) {
        RK_CFG_LOG("The path to the config was empty. Using the default RK Logger config." << std::endl);
        return;
    } 
    if (!std::filesystem::exists(path)) {
        RK_CFG_LOG("A config file didn't exist at the provided path " << path << ". Using the default RK Logger config" << std::endl);
        return;
    }

    RK_CFG_LOG("Trying to open RK Logger config file at path " << path << "" << std::endl);
    std::ifstream configFile(path);
    if (!configFile) {
        RK_CFG_LOG("Could not open RK Logger config file. Either one wasn't provided or the path provided was invalid. Using default RK Logger config" << std::endl);
        return;
    }
    else {
        RK_CFG_LOG("Successfully opened the RK Logger config file" << std::endl);
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
            RK_CFG_LOG("Key \"" << configKey << "\" was not valid. Not updating." << std::endl);
            continue;
        }

        ConfigValue configValue = line.substr(equalsIndex + 1);
        // Remove new line at end of configValue, if it exists
        const size_t newLineIndex = configValue.find_last_of("\n");
        if (newLineIndex != std::string::npos && newLineIndex == configValue.size() - 1) {
            configValue = configValue.substr(0, configValue.size());
        }
        if (!isKeyAndValueValid(configKey, configValue)) {
            RK_CFG_LOG("Value \"" << configValue << "\" for key \"" << configKey << "\" was not valid. Not updating." << std::endl);
            continue;
        }

        RK_CFG_LOG("Updating config key \"" << configKey << "\" with value \"" << configValue << "\"" << std::endl);
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
    static Config configuration(std::move(defaultConfig), std::move(validKeyValues));
    return configuration;
}

} // namespace config
} // namespace rk
