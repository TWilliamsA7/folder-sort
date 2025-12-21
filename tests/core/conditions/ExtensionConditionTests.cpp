// tests/core/conditions/ExtensionConditionTests.cpp

#include <gtest/gtest.h>
#include "core/conditions/ExtensionCondition.hpp"

TEST(ExtensionConditionTests, MatchesCorrectExtensions) {
    ExtensionCondition cond({ ".txt", ".docx" });

    FileInfo file1, file2, file3;
    file1.path = "example.txt";
    file2.path = "test.docx";
    file3.path = "image.jpg";

    EXPECT_EQ(cond.check(file1), true);
    EXPECT_EQ(cond.check(file2), true);
    EXPECT_EQ(cond.check(file3), false);
}

TEST(ExtensionConditionTests, MatchesCaseInsensitiveExtension) {
    ExtensionCondition cond({ ".JPG" });

    FileInfo file;
    file.path = "image.jpg";

    EXPECT_EQ(cond.check(file), true);
}

TEST(ExtensionConditionTests, NormalizesExtensionWithoutDot) {
    ExtensionCondition cond({ "log" });

    FileInfo file;
    file.path = "december.log";

    EXPECT_EQ(cond.check(file), true);
}