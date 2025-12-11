cmake_minimum_required(VERSION 3.20)
project(foldersort VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(FetchContent)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 10.0.0
)

FetchContent_Declare(spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.11.0
)

# Make both available (you missed fmt here)
FetchContent_MakeAvailable(fmt spdlog)

add_executable(foldersort src/main.cpp)

# Link libraries so includes and linking are handled automatically
target_link_libraries(foldersort PRIVATE fmt::fmt spdlog::spdlog)

# keep your local include dir if you need it
target_include_directories(foldersort PRIVATE ${CMAKE_SOURCE_DIR}/include)
