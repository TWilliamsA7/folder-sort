// tests/logging/LoggerTest.hpp

#include <gtest/gtest.h>
#include <filesystem>

#include "logging/Logger.hpp"

class LoggerTest : public ::testing::Test {
    protected:
        std::string logDir = "test_logs";
        std::filesystem::path logFile;

        void SetUp() override {
            logFile = logging::Logger::InitForTests(logDir);
        }

        void TearDown() override {
            logging::Logger::Shutdown();
            std::filesystem::remove_all(logDir);
        }
};
