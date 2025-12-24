// tests/logging/LogTests.cpp

#include "../helpers/TestLogger.hpp"
#include <fstream>

TEST_F(LoggerTest, WritesInfoMessageToFile) {
    logging::Logger::Info("Hello from test");

    EXPECT_TRUE(std::filesystem::exists(logFile));

    std::ifstream in(logFile);
    std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    EXPECT_NE(contents.find("Hello from test"), std::string::npos);
}