#ifndef CONFIG_TESTS_H
#define CONFIG_TESTS_H

#include <rk_logger/logger.h>
#include <rk_logger/config.h>
#include <rk_logger_tests/test_base.h>

namespace rk_logger_tests {
namespace config_tests {

inline const std::string INVALID_KEY_GENERIC = "INVALID_KEY";
inline const std::string INVALID_VALUE_GENERIC = "INVALID_VALUE";
inline const std::string NO_KEY = "";
inline const std::string RK_CFG_FILE_MODIFIED = "rk_config_modified.yaml";
inline const std::string RK_CFG_FILE_NON_EXISTENT = "rk_config_non_existent.yaml";
inline const std::string RK_CFG_FILE_DIR = "test_data";
inline const std::string RK_CFG_FILE_KEY_VALUE_SEPARATOR = ":";
inline const std::string TEMP_DIR = "temp";

class ConfigTest : public Base {
protected:
    rk::config::Config& config = rk::config::getInstance();
};

struct ConfigKeyValueTestParam : public rk_logger_tests::BaseParam {
    ConfigKeyValueTestParam(
        const std::string description,
        const rk::config::ConfigKey key,
        const bool isKeyValid,
        const rk::config::ConfigValue value,
        const bool isValueValid,
        const std::string keyForSuffix = "",
        const std::string valueForSuffix = ""
    ) : BaseParam(description), key(key), isKeyValid(isKeyValid), value(value), isValueValid(isValueValid), keyForSuffix(keyForSuffix), valueForSuffix(valueForSuffix) {};

    const rk::config::ConfigKey key;
    const bool isKeyValid;
    const rk::config::ConfigValue value;
    const bool isValueValid;

    // The values below are Google Test suffix-friendly values that get appended to the test name in the output
    const std::string keyForSuffix;
    const std::string valueForSuffix;
};

class ConfigKeyValidTest : public ConfigTest, public ::testing::WithParamInterface<ConfigKeyValueTestParam> {};
class ConfigKeyValueValidTest : public ConfigTest, public ::testing::WithParamInterface<ConfigKeyValueTestParam> {};
class ConfigGetKeyValueTest : public ConfigTest, public ::testing::WithParamInterface<ConfigKeyValueTestParam> {};
class ConfigSetKeyValueTest : public ConfigTest, public ::testing::WithParamInterface<ConfigKeyValueTestParam> {};
class ConfigGetValidKeyValuesTest : public ConfigTest {};

class ConfigFileTest : public ConfigTest {
protected:
    const std::string USING_DEFAULT_MESSAGE = "Using the default RK Logger config";
    const std::string CFG_FILE_DIDNT_EXIST_MESSAGE = "A config file didn't exist at the provided path";
    const std::string TRYING_TO_OPEN_MSG = "Trying to open RK Logger config file at path";
    const std::string SUCCESSFULLY_OPENED_MSG = "Successfully opened the RK Logger config file";
    std::filesystem::path cfgFilePath; /**< The default config file that is copied to the executable's location as part of test setup */

    void SetUp() override {
        redirectStdCout();
        SCOPED_TRACE("Copying config file to the executable's location");
        cfgFilePath = std::filesystem::path(RK_LOGGER_TESTS_BINARY_DIR)/rk::config::CONFIG_FILE_NAME;
        std::filesystem::copy_file(std::filesystem::path(RK_LOGGER_TESTS_BASE_DIR)/RK_CFG_FILE_DIR/rk::config::CONFIG_FILE_NAME, cfgFilePath);
    }
    
    void TearDown() override {
        undoRedirectStdCout();
        SCOPED_TRACE("Removing config file that was copied to the executable's location");
        std::filesystem::remove(cfgFilePath);
    }
};

struct ConfigFileTestParam : public rk_logger_tests::BaseParam {
    using ConfigFileKeyValues = std::unordered_map<std::string, std::string>;
    ConfigFileTestParam(const std::string description, ConfigFileKeyValues keyValues) : BaseParam(description), keyValues(keyValues) {};

    const ConfigFileKeyValues keyValues;
};

class ConfigFileTestParameterized : public ConfigTest, public ::testing::WithParamInterface<ConfigFileTestParam>  {
protected:
    void SetUp() override {
        auto param = GetParam();
        modifyConfigFile(param.keyValues);
    }

    void TearDown() override {
        std::filesystem::remove_all(std::filesystem::path(RK_LOGGER_TESTS_BINARY_DIR)/TEMP_DIR);
    }
    
    void modifyConfigFile(const ConfigFileTestParam::ConfigFileKeyValues& keyValues) {
        SCOPED_TRACE("Opening CFG base file");
        std::filesystem::path path = std::filesystem::path(RK_LOGGER_TESTS_BASE_DIR)/RK_CFG_FILE_DIR/rk::config::CONFIG_FILE_NAME;
        std::fstream configFileBase(path, std::ios::in);
        verifyConfigFile(configFileBase);
        
        SCOPED_TRACE("Creating new CFG file");
        std::filesystem::create_directory(std::filesystem::path(RK_LOGGER_TESTS_BINARY_DIR)/TEMP_DIR);
        configFileNewPath = std::filesystem::path(RK_LOGGER_TESTS_BINARY_DIR)/TEMP_DIR/RK_CFG_FILE_MODIFIED;
        std::fstream configFileNew(configFileNewPath, std::ios::out);
        verifyConfigFile(configFileNew);

        std::string line;
        while (std::getline(configFileBase, line)) {
            for (const auto& pair : keyValues) {
                auto iter = line.find(pair.first);
                if (iter != std::string::npos) {
                    line = replaceConfigValue(line, pair.second);
                }
            }
            configFileNew << line + "\n";
        }

        configFileBase.close();
        configFileNew.close();
    }

    void verifyConfigFile(const std::fstream& file) const {
        ASSERT_TRUE(file.good());
    }

    std::string replaceConfigValue(std::string line, rk::config::ConfigValue value) const {
        size_t pos = line.find(RK_CFG_FILE_KEY_VALUE_SEPARATOR);
        if (pos != std::string::npos) {
            line = line.substr(0, pos + 1) + " " + value;
        }

        return line;
    }

    std::filesystem::path configFileNewPath;
};

} // namespace config_tests
} // namespace rk_logger_tests

#endif // #ifndef CONFIG_TEST_H
