#include "folder_sort/scan.hpp"
#include <gtest/gtest.h>

TEST(ScannerTests, BasicReturnZero) {
    // Call the small function with a known input
    int rc = folder_sort::scan_directory(".");
    EXPECT_EQ(rc, 0);
}
