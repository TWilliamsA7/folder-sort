// tests/core/filesystem/FilesystemScannerWinOnlyTests.cpp

#ifdef _WIN32

#include <gtest/gtest.h>
#include <windows.h>
#include "../tests/helpers/TestFilesystem.hpp"
#include "core/filesystem/FilesystemScanner.hpp"
#include "../tests/core/filesystem/FilesystemScannerTest.hpp"

TEST_F(FilesystemScannerTest, SkipsHiddenFilesByDefault) {
    TestTree dir("scanner_win_hidden");

    auto hidden = dir.file("hidden.txt");

    SetFileAttributesW(hidden.wstring().c_str(), FILE_ATTRIBUTE_HIDDEN);

    ScanOptions opts;

    FilesystemScanner scanner(dir.root, opts);
    auto result = scanner.scan();

    EXPECT_TRUE(result.files.empty());
}


TEST_F(FilesystemScannerTest, IncludesHiddenFilesWhenRequested) {
    TestTree dir("scanner_win_hidden");

    auto hidden = dir.file("hidden.txt");

    SetFileAttributesW(hidden.wstring().c_str(), FILE_ATTRIBUTE_HIDDEN);

    ScanOptions opts;
    opts.include_hidden = true;

    FilesystemScanner scanner(dir.root, opts);
    auto result = scanner.scan();

    EXPECT_EQ(result.files.size(), 1);
}


#endif