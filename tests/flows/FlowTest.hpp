// tests/flows/FlowTest.hpp

#include <gtest/gtest.h>
#include <filesystem>

#include "logging/Logger.hpp"

class FlowTest : public ::testing::Test {
    protected:
        std::string logDir = "test_flow_logs";
        std::filesystem::path logFile;

        void SetUp() override {
            logFile = logging::Logger::InitForTests(logDir);
        }

        void TearDown() override {
            logging::Logger::Shutdown();
            std::filesystem::remove_all(logDir);
        }
};
