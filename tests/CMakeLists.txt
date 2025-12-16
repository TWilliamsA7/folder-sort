# Ensure GTest targets exist before proceeding
if(NOT TARGET GTest::gtest_main)
    # If FetchContent didn't create the alias GTest::gtest_main, map it manually
    if(TARGET gtest_main)
        add_library(GTest::gtest_main ALIAS gtest_main)
    else()
        message(FATAL_ERROR "GTest target not found.")
    endif()
endif()

# --- Unit Test Executable ---
add_executable(unit_tests
    test_main.cpp
    test/scanner_tests.cpp  # Based on your image structure
)

# MSYS2/MinGW often needs Threads explicitly linked for GTest
find_package(Threads REQUIRED)

target_link_libraries(unit_tests PRIVATE 
    folder_sort_lib 
    GTest::gtest_main
    Threads::Threads
)

# --- Discovery ---
# This integrates with CTest and VSCode Test Explorer automatically
include(GoogleTest)
gtest_discover_tests(unit_tests)