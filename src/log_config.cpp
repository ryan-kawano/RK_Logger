/**
 * @file log_config.cpp
 * @brief Source file for the logging config.
 */
#include <iostream>
#include <fstream>

#include "rk_logger/log_config.h"

namespace rk {
namespace config {

PossibleValuesMap dateFormatPossibleValues = {
    { "MM_DD_YYYY", 0u }, // i.e., Feb 4, 2025 is formatted as [02|Feb]-04-2025
    { "DD_MM_YYYY", 1u }, // i.e., Feb 4, 2025 is formatted as 04-[02|Feb]-2025
    { "YYYY_MM_DD", 2u } // i.e., Feb 4, 2025 is formatted as 2025-[02|Feb]-04
};

PossibleValuesMap monthFormatPossibleValues = {
    { "MONTH_NUM", 0u }, // Prints the months name, e.g., Jan, Feb, etc.
    { "MONTH_NAME", 1u } // Prints the month's number, e.g., 01, 02, etc.
};

ConfigMap configMap = {
    { "date_format", std::make_tuple(dateFormatPossibleValues, dateFormatPossibleValues.at("MM_DD_YYYY")) },
    { "month_format", std::make_tuple(monthFormatPossibleValues, monthFormatPossibleValues.at("MONTH_NUM")) }
};

void getLoggingConfig(const std::filesystem::path& path) {
    std::cout << "Getting RK Logger config file" << std::endl;

    if (path.empty()) {
        std::cout << "The path to the config was empty. Using the default RK Logger config." << std::endl;
        return;
    } 
    
    if (!std::filesystem::exists(path)) {
        std::cout << "A config file didn't exist at the provided path. Using the default RK Logger config" << std::endl;
        return;
    }

    std::cout << "Trying to open RK Logger config file at path " << path << "" << std::endl;
    std::ifstream configFile(path);
    if (!configFile) {
        std::cout << "Could not open RK Logger config file. Either one wasn't provided or the path provided was wrong. Using default RK Logger config" << std::endl;
        return;
    }
    else {
        std::cout << "Successfully opened the RK Logger config file" << std::endl;
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

        const std::string configKey = line.substr(0, equalsIndex);

        // Check if the user-provided key is a valid logger setting
        auto configKeyIter = rk::config::configMap.find(configKey);
        if (configKeyIter == rk::config::configMap.end()) {
            std::cout << "Key \"" << configKey << "\" was not valid. Skipping" << std::endl;
            continue;
        }

        std::string configValueStr = line.substr(equalsIndex + 1);

        // Remove new line at end of configValue, if it exists
        const size_t newLineIndex = configValueStr.find_last_of("\n");
        if (newLineIndex != std::string::npos && newLineIndex == configValueStr.size() - 1) {
            configValueStr = configValueStr.substr(0, configValueStr.size());
        }

        // Check if the user-provided value is a valid logger value
        const PossibleValuesMap& valuesMap = std::get<0>(configKeyIter->second);
        auto configValueIter = valuesMap.find(configValueStr);
        if (configValueIter == valuesMap.end()) {
            std::cout << "Value \"" << configValueStr << "\" for key was not valid. Skipping" << std::endl;
            continue;
        }

        const ActualValue configValue = configValueIter->second;
        std::cout << "Updating config with " << configValueStr << " (numerical value: " << std::to_string(configValue) << ")" << std::endl;
        rk::config::updateConfigValue(configKeyIter, configValue);
    }
}

void updateConfigValue(const ConfigMap::iterator& configKeyIter, const ActualValue newValue) {
    ActualValue& value =  std::get<1>(configKeyIter->second); // Access the current config's value
    value = newValue;
};

} // namespace config
} // namespace rk
