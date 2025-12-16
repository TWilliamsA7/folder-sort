cmake_minimum_required(VERSION 3.24) # 3.24+ handles FetchContent policies much better
project(folder-sort VERSION 0.1 LANGUAGES CXX)

# --- Options ---
option(BUILD_TESTS "Build unit tests" ON)
option(USE_SYSTEM_DEPENDENCIES "Prefer system-installed libraries" OFF)

# --- Standard Settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Enforces -std=c++20 instead of -std=gnu++20
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# FIX: Add this line to suppress errors from dependencies using old CMake versions
set(CMAKE_POLICY_VERSION_MINIMUM "3.5")

# Export compile commands for VSCode IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Dependencies ---
include(FetchContent)

# 1. fmt
if(NOT USE_SYSTEM_DEPENDENCIES)
    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        10.1.1
    )
endif()

# 2. spdlog
if(NOT USE_SYSTEM_DEPENDENCIES)
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.12.0
    )
endif()

# 3. CLI11
if(NOT USE_SYSTEM_DEPENDENCIES)
    # CLI11
    FetchContent_Declare(
      CLI11
      GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
      GIT_TAG v2.4.1  # CHANGED FROM v2.3.2 to v2.4.1
    )
endif()

# 4. yaml-cpp
if(NOT USE_SYSTEM_DEPENDENCIES)
    # Turn off yaml-cpp tests to speed up build
    set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG        0.8.0 # 0.8.0 fixes many modern C++ warnings over 0.7.0
    )
endif()

# 5. GoogleTest (Only if testing)
if(BUILD_TESTS AND NOT USE_SYSTEM_DEPENDENCIES)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0 # Always pin to a release tag, not a hash
    )
    # Prevent GTest from overriding parent compile options (important for MSYS2)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

# --- Make Available / Find Package ---
if(USE_SYSTEM_DEPENDENCIES)
    find_package(fmt REQUIRED)
    find_package(spdlog REQUIRED)
    find_package(CLI11 REQUIRED)
    find_package(yaml-cpp REQUIRED)
    if(BUILD_TESTS)
        find_package(GTest REQUIRED)
    endif()
else()
    # This automatically populates and calls add_subdirectory
    FetchContent_MakeAvailable(fmt spdlog CLI11 yaml-cpp)
    if(BUILD_TESTS)
        FetchContent_MakeAvailable(googletest)
    endif()
endif()

# --- Add Project Subdirectories ---
add_subdirectory(src)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()