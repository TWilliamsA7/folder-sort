// tests/core/conditions/ContentConditionTests.cpp

#include <gtest/gtest.h>
#include "core/conditions/ContentCondition.hpp"
#include "../tests/helpers/TestFilesystem.hpp"

TEST(ContentConditionTests, MatchesSimpleFileContent) {
    const std::regex pattern(".+apple.+");
    ContentCondition cond(pattern);

    auto tree = TestTree("scanner_file");
    auto file = tree.file("test.txt");
    std::ofstream(file) << " apple123 ";
    auto file2 = tree.file("test2.txt");

    FileInfo f1, f2;
    f1.path = file;
    f2.path = file2;

    EXPECT_EQ(cond.check(f1), true);
    EXPECT_EQ(cond.check(f2), false);

}
