// tests/logging/LogTests.cpp

#include "../helpers/TestLogger.hpp"
#include <fstream>

TEST_F(LoggerTest, LogFileIsCreated) {
    auto log = logging::Logger::Get("tests");
    log->info("Hello from test");

    EXPECT_TRUE(std::filesystem::exists(logFile));
}

TEST_F(LoggerTest, LogFileContainsMessage) {
    auto log = logging::Logger::Get("tests");
    log->info("Hello from test");

    std::ifstream in(logFile);
    ASSERT_TRUE(in.is_open());

    std::string contents(
        (std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>());

    EXPECT_NE(contents.find("Hello from test"), std::string::npos)
        << "Actual contents: " << contents;
}
