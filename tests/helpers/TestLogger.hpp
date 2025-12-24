// tests/helpers/TestLogger.hpp

#include <gtest/gtest.h>
#include "logging/Logger.hpp"
#include <filesystem>

class LoggerTest : public ::testing::Test {
    protected:
        std::string logDir;
        std::string logFile;

        void SetUp() override {
            logDir = "test_logs";
            logFile = fmt::format(
                "{}/logger_test_{}.log",
                logDir,
                ::testing::UnitTest::GetInstance()->random_seed()
            );

            std::filesystem::create_directories(logDir);
            logging::Logger::InitForTests(logFile);
        }

        void TearDown() override {
            logging::Logger::Shutdown();
            std::filesystem::remove_all(logDir);
        }
};