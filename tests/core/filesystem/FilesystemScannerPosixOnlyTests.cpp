// tests/core/filesystem/FilesystemScannerPosixOnlyTests.cpp

#ifndef _WIN32

#include <gtest/gtest.h>
#include <sys/stat.h>
#include "core/filesystem/FilesystemScanner.hpp"
#include "../tests/helpers/TestFilesystem.hpp"


TEST(ScannerPosix, SkipsDotfilesByDefault) {
    TestTree dir("scanner_posix_hidden");

    dir.file(".secret");

    ScanOptions opts;
    opts.include_hidden = false;

    FilesystemScanner scanner(dir.root, opts);
    auto result = scanner.scan();

    EXPECT_TRUE(result.files.empty());
}

TEST(ScannerPosix, RecordsPermissionDeniedError) {
    TestTree dir("scanner_posix_perm");

    auto blocked = dir.dir("blocked");

    chmod(blocked.string().c_str(), 0000);

    ScanOptions opts;
    opts.allow_permission_errors = true;

    FilesystemScanner scanner(dir.root, opts);
    auto result = scanner.scan();

    chmod(blocked.string().c_str(), 0755); // cleanup

    EXPECT_TRUE(result.has_errors());
    EXPECT_EQ(result.errors[0].type, ScanErrorType::PermissionDenied);
}

#endif