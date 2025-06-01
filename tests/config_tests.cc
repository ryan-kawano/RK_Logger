#include <rk_logger/logger.h>
#include <rk_logger/config.h>
#include "config_tests.h"

namespace rk_logger_tests {
namespace config_tests {

TEST(GetConfigTest, GetConfigInstance) {
    SCOPED_TRACE("Getting Config instance");
    rk::config::Config& config1 = rk::config::getInstance();
    ASSERT_NE(&config1, nullptr);
}

// Getting a Config instance should always return the same instance
TEST(GetConfigTest, OnlyOneConfigInstance) {
    SCOPED_TRACE("Getting Config instance");
    rk::config::Config& config1 = rk::config::getInstance();
    ASSERT_NE(&config1, nullptr);

    SCOPED_TRACE("Getting Config instance again");
    rk::config::Config& config2 = rk::config::getInstance();
    ASSERT_EQ(&config1, &config2);
}

TEST_P(ConfigKeyValidTest, IsKeyValid) {
    auto param = GetParam();
    SCOPED_TRACE("Checking validity of a key");
    auto result = config.isKeyValid(param.key);
    ASSERT_EQ(result, param.isKeyValid);
}

INSTANTIATE_TEST_SUITE_P(ConfigIsKeyValid,
    ConfigKeyValidTest,
    testing::Values(
        // Valid keys
        ConfigKeyValueTestParam("", rk::config::date_format::KEY, true, "", false),
        ConfigKeyValueTestParam("", rk::config::month_format::KEY, true, "", false),
        ConfigKeyValueTestParam("", rk::config::hour_format::KEY, true, "", false),
        ConfigKeyValueTestParam("", rk::config::write_to_log_file::KEY, true, "", false),

        // Invalid keys
        ConfigKeyValueTestParam("", INVALID_KEY_GENERIC, false, "", false),
        ConfigKeyValueTestParam("", "!@#$%^!", false, "", false, "special_characters"),
        ConfigKeyValueTestParam("", "DATE_FORMAT", false, "", false),
        ConfigKeyValueTestParam("", "month format", false, "", false, "month_format_with_space"),
        ConfigKeyValueTestParam("", "hour+format", false, "", false, "hour_format_with_plus_symbol"),
        ConfigKeyValueTestParam("", "a", false, "", false)
    ),
    [](const testing::TestParamInfo<ConfigKeyValueTestParam>& info) {
        return std::string(!info.param.description.empty() ? info.param.description : "") + (info.param.keyForSuffix.empty() ? info.param.key : info.param.keyForSuffix) + "_" + (info.param.isKeyValid ? "isValid" : "isInvalid");
    }
);

TEST_P(ConfigKeyValueValidTest, IsKeyAndValueValid) {
    auto param = GetParam();
    SCOPED_TRACE("Checking validity of key and value");
    auto result = config.isKeyAndValueValid(param.key, param.value);
    ASSERT_EQ(result, param.isKeyValid && param.isValueValid);
}

INSTANTIATE_TEST_SUITE_P(ConfigIsKeyValueValid,
    ConfigKeyValueValidTest,
    testing::Values(
        // Valid values for a given key
        ConfigKeyValueTestParam("", rk::config::date_format::KEY, true, rk::config::date_format::DD_MM_YYYY, true),
        ConfigKeyValueTestParam("", rk::config::date_format::KEY, true, rk::config::date_format::MM_DD_YYYY, true),
        ConfigKeyValueTestParam("", rk::config::date_format::KEY, true, rk::config::date_format::YYYY_MM_DD, true),
        ConfigKeyValueTestParam("", rk::config::month_format::KEY, true, rk::config::month_format::MONTH_NAME, true),
        ConfigKeyValueTestParam("", rk::config::month_format::KEY, true, rk::config::month_format::MONTH_NUM, true),
        ConfigKeyValueTestParam("", rk::config::hour_format::KEY, true, rk::config::hour_format::TWELVE_HOUR, true),
        ConfigKeyValueTestParam("", rk::config::hour_format::KEY, true, rk::config::hour_format::TWENTY_FOUR_HOUR, true),
        ConfigKeyValueTestParam("", rk::config::write_to_log_file::KEY, true, rk::config::write_to_log_file::DISABLE, true),
        ConfigKeyValueTestParam("", rk::config::write_to_log_file::KEY, true, rk::config::write_to_log_file::ENABLE, true),

        // Invalid values for a given key
        ConfigKeyValueTestParam("", rk::config::date_format::KEY, true, INVALID_KEY_GENERIC, false),
        ConfigKeyValueTestParam("", rk::config::date_format::KEY, true, "YY_MM_DD", false), // 2-digit YY not supported
        ConfigKeyValueTestParam("", rk::config::date_format::KEY, true, "DD_YYYY_MM", false), // Correct format, but not a valid value
        ConfigKeyValueTestParam("", rk::config::date_format::KEY, true, rk::config::month_format::MONTH_NAME, false), // Value from another key
        ConfigKeyValueTestParam("", rk::config::date_format::KEY, true, rk::config::hour_format::TWELVE_HOUR, false), // Value from another key
        ConfigKeyValueTestParam("", rk::config::date_format::KEY, true, rk::config::write_to_log_file::DISABLE, false), // Value from another key
        ConfigKeyValueTestParam("", rk::config::month_format::KEY, true, INVALID_KEY_GENERIC, false),
        ConfigKeyValueTestParam("", rk::config::month_format::KEY, true, "month_name", false), // Lowercase version of valid value
        ConfigKeyValueTestParam("", rk::config::month_format::KEY, true, rk::config::date_format::DD_MM_YYYY, false), // Value from another key
        ConfigKeyValueTestParam("", rk::config::month_format::KEY, true, rk::config::hour_format::TWELVE_HOUR, false), // Value from another key
        ConfigKeyValueTestParam("", rk::config::month_format::KEY, true, rk::config::write_to_log_file::DISABLE, false), // Value from another key
        ConfigKeyValueTestParam("", rk::config::hour_format::KEY, true, INVALID_KEY_GENERIC, false),
        ConfigKeyValueTestParam("", rk::config::hour_format::KEY, true, "twelve_hour", false), // Lowercase version of valid value
        ConfigKeyValueTestParam("", rk::config::hour_format::KEY, true, rk::config::month_format::MONTH_NAME, false), // Value from another key
        ConfigKeyValueTestParam("", rk::config::hour_format::KEY, true, rk::config::date_format::DD_MM_YYYY, false), // Value from another key
        ConfigKeyValueTestParam("", rk::config::hour_format::KEY, true, rk::config::write_to_log_file::DISABLE, false), // Value from another key
        ConfigKeyValueTestParam("", rk::config::write_to_log_file::KEY, true, INVALID_KEY_GENERIC, false),
        ConfigKeyValueTestParam("", rk::config::write_to_log_file::KEY, true, "enable", false), // Lowercase version of valid value
        ConfigKeyValueTestParam("", rk::config::write_to_log_file::KEY, true, rk::config::month_format::MONTH_NAME, false), // Value from another key
        ConfigKeyValueTestParam("", rk::config::write_to_log_file::KEY, true, rk::config::hour_format::TWELVE_HOUR, false), // Value from another key
        ConfigKeyValueTestParam("", rk::config::write_to_log_file::KEY, true, rk::config::date_format::DD_MM_YYYY, false), // Value from another key

        // Invalid keys
        ConfigKeyValueTestParam("", INVALID_KEY_GENERIC, false, "", false),
        ConfigKeyValueTestParam("", "!@#$%^!", false, "", false, "special_characters"),
        ConfigKeyValueTestParam("", "DATE_FORMAT", false, "", false),
        ConfigKeyValueTestParam("", "month format", false, "", false, "month_format_with_space"),
        ConfigKeyValueTestParam("", "hour+format", false, "", false, "hour_format_with_plus_symbol"),
        ConfigKeyValueTestParam("", "a", false, "", false)
    ),
    [](const testing::TestParamInfo<ConfigKeyValueTestParam>& info) {
        return std::string(!info.param.description.empty() ? info.param.description : "") + (info.param.keyForSuffix.empty() ? info.param.key : info.param.keyForSuffix) + "_" + (info.param.isKeyValid ? "isValid" : "isInvalid") + "_" + info.param.value + "_" + (info.param.isValueValid ? "isValid" : "isInvalid");
    }
);

// Retrieving a Config value with a valid key should always return something, in this case a default value
TEST_P(ConfigGetKeyValueTest, GetConfigValueDefault) {
    auto param = GetParam();
    const std::string value = config.getConfigValueByKey(param.key);
    ASSERT_EQ(value.empty(), param.isKeyValid ? false : true);
}

INSTANTIATE_TEST_SUITE_P(ConfigGetKeyValueTest,
    ConfigGetKeyValueTest,
    testing::Values(
        // Valid keys
        ConfigKeyValueTestParam("", rk::config::date_format::KEY, true, "", false),
        ConfigKeyValueTestParam("", rk::config::month_format::KEY, true, "", false),
        ConfigKeyValueTestParam("", rk::config::hour_format::KEY, true, "", false),
        ConfigKeyValueTestParam("", rk::config::write_to_log_file::KEY, true, "", false),

        // Invalid keys
        ConfigKeyValueTestParam("", INVALID_KEY_GENERIC, false, "", false),
        ConfigKeyValueTestParam("", NO_KEY, false, "", false, "NO_KEY"),
        ConfigKeyValueTestParam("", "YY_MM_DD", false, "", false)
    ),
    [](const testing::TestParamInfo<ConfigKeyValueTestParam>& info) {
        return std::string(!info.param.description.empty() ? info.param.description : "") + (info.param.keyForSuffix.empty() ? info.param.key : info.param.keyForSuffix) + "_" + (info.param.isKeyValid ? "isValid" : "isInvalid");
    }
);

TEST_P(ConfigSetKeyValueTest, SetConfigValue) {
    auto param = GetParam();
    ASSERT_EQ(config.isKeyValid(param.key), param.isKeyValid);
    ASSERT_EQ(config.isKeyAndValueValid(param.key, param.value), param.isKeyValid && param.isValueValid);

    SCOPED_TRACE("Setting Config value");
    config.setConfigValue(param.key, param.value);

    SCOPED_TRACE("Getting config value that was just set");
    const std::string value = config.getConfigValueByKey(param.key);
    if (param.isKeyValid && param.isValueValid) {
        ASSERT_EQ(value, param.value);
    }
}

INSTANTIATE_TEST_SUITE_P(ConfigSetKeyValueTest,
    ConfigSetKeyValueTest,
    testing::Values(
        // Valid keys and values
        ConfigKeyValueTestParam("valid_key_and_value", rk::config::date_format::KEY, true, rk::config::date_format::YYYY_MM_DD, true),
        ConfigKeyValueTestParam("valid_key_and_value", rk::config::month_format::KEY, true, rk::config::month_format::MONTH_NAME, true),
        ConfigKeyValueTestParam("valid_key_and_value", rk::config::hour_format::KEY, true, rk::config::hour_format::TWENTY_FOUR_HOUR, true),
        ConfigKeyValueTestParam("valid_key_and_value", rk::config::write_to_log_file::KEY, true, rk::config::write_to_log_file::DISABLE, true),

        // Valid keys, but invalid values
        ConfigKeyValueTestParam("valid_key_invalid_value", rk::config::date_format::KEY, true, INVALID_VALUE_GENERIC, false),
        ConfigKeyValueTestParam("valid_key_invalid_value", rk::config::month_format::KEY, true, INVALID_VALUE_GENERIC, false),
        ConfigKeyValueTestParam("valid_key_invalid_value", rk::config::hour_format::KEY, true, INVALID_VALUE_GENERIC, false),
        ConfigKeyValueTestParam("valid_key_invalid_value", rk::config::write_to_log_file::KEY, true, INVALID_VALUE_GENERIC, false),

        // Invalid keys
        ConfigKeyValueTestParam("invalid_key", INVALID_KEY_GENERIC, false, "", false),
        ConfigKeyValueTestParam("invalid_key", NO_KEY, false, "", false, "NO_KEY"),
        ConfigKeyValueTestParam("invalid_key", "YY_MM_DD", false, "", false)
    ),
    [](const testing::TestParamInfo<ConfigKeyValueTestParam>& info) {
        return std::string(!info.param.description.empty() ? info.param.description : "") + "_" + (info.param.keyForSuffix == "" ? info.param.key : info.param.keyForSuffix);
    }
);

TEST_F(ConfigFileTest, ParseLoggingConfigBlankPath) {
    SCOPED_TRACE("Calling parseLoggingConfig() with a blank path");
    config.parseLoggingConfig(std::filesystem::path());
    ASSERT_NE(logOutput.str().find(USING_DEFAULT_MESSAGE), std::string::npos);

    SCOPED_TRACE("Verifying set config values");
    for (const auto keyValuePair : rk::config_internal::defaultConfig) {
        ASSERT_EQ(keyValuePair.second, config.getConfigValueByKey(keyValuePair.first));
    }
}

TEST_F(ConfigFileTest, ParseLoggingConfigNonExistent) {
    std::filesystem::path cfgPath(RK_CFG_FILE_DIR + RK_CFG_FILE_NON_EXISTENT);
    SCOPED_TRACE("Verifying that Config file is non-existent");
    ASSERT_TRUE(!std::filesystem::exists(cfgPath));

    SCOPED_TRACE("Calling parseLoggingConfig() with a non-existent path");
    config.parseLoggingConfig(cfgPath);
    ASSERT_NE(logOutput.str().find(CFG_FILE_DIDNT_EXIST_MESSAGE), std::string::npos);
    
    SCOPED_TRACE("Verifying set config values");
    for (const auto& keyValuePair : rk::config_internal::defaultConfig) {
        ASSERT_EQ(keyValuePair.second, config.getConfigValueByKey(keyValuePair.first));
    }
}

TEST_F(ConfigFileTest, ParseLoggingConfigNoArgumentProvided) {
    SCOPED_TRACE("Verifying that a config file exists at the same location as the executable");
    ASSERT_TRUE(std::filesystem::exists("./" + rk::config::CONFIG_FILE_NAME));

    SCOPED_TRACE("Calling parseLoggingConfig() without providing any arguments");
    config.parseLoggingConfig();
    ASSERT_NE(logOutput.str().find(TRYING_TO_OPEN_MSG), std::string::npos);
    ASSERT_NE(logOutput.str().find(SUCCESSFULLY_OPENED_MSG), std::string::npos);

    SCOPED_TRACE("Verifying set config values");
    for (const auto& keyValuePair : rk::config_internal::defaultConfig) {
        ASSERT_EQ(keyValuePair.second, config.getConfigValueByKey(keyValuePair.first));
    }
}

TEST_P(ConfigFileTestParameterized, ParseLoggingConfig) {
    auto param = GetParam();

    SCOPED_TRACE("Calling parseLoggingConfig()");
    rk::config::getInstance().parseLoggingConfig(configFileNewPath);

    SCOPED_TRACE("Verifying set config values");
    for (const auto& keyValuePair : param.keyValues) {
        if (rk::config::getInstance().isKeyAndValueValid(keyValuePair.first, keyValuePair.second)) {
            ASSERT_EQ(keyValuePair.second, rk::config::getInstance().getConfigValueByKey(keyValuePair.first));
        }
    }
}

INSTANTIATE_TEST_SUITE_P(ConfigFileTestParameterized,
    ConfigFileTestParameterized,
    testing::Values(
        ConfigFileTestParam(
            "non_default_values",
            {
                { rk::config::date_format::KEY, rk::config::date_format::YYYY_MM_DD },
                { rk::config::month_format::KEY, rk::config::month_format::MONTH_NAME },
                { rk::config::hour_format::KEY, rk::config::hour_format::TWENTY_FOUR_HOUR },
                { rk::config::write_to_log_file::KEY, rk::config::write_to_log_file::DISABLE }
            }
        ),
        ConfigFileTestParam(
            "invalid_values",
            {
                { rk::config::date_format::KEY, "YY_MM_DD" },
                { rk::config::month_format::KEY, "!@#$%^&" },
                { rk::config::hour_format::KEY, "5" },
                { rk::config::write_to_log_file::KEY, "enable" }
            }
        )
    ),
    [](const testing::TestParamInfo<ConfigFileTestParam>& info) {
        return info.param.description;
    }
);

TEST_F(ConfigGetValidKeyValuesTest, GetValidKeyValues) {
    rk::config::ValidKeyValuesMap& validKeyValues = config.getValidKeyValues();
    ASSERT_FALSE(validKeyValues.empty());
    for (const auto& pair : validKeyValues) {
        ASSERT_FALSE(pair.second.empty());
    }
}

} // namespace config_tests
} // namespace rk_logger_tests
