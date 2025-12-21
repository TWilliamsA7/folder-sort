// tests/config/ConfigLoaderTests.cpp

#include <gtest/gtest.h>

#include "config/ConfigLoader.hpp"

TEST(ConfigLoaderTests, ThrowWithoutRules) {
    std::string path = "test.yaml";
    ConfigLoader loader(path);

    EXPECT_THROW(loader.load(), std::exception);
}