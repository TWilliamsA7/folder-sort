# --- Library Target ---
add_library(folder_sort_lib STATIC)

add_subdirectory(core)
add_subdirectory(config)
add_subdirectory(logging)

# Define include directories
# BUILD_INTERFACE allows headers to be found during build
# INSTALL_INTERFACE allows headers to be found if you install this lib
target_include_directories(folder_sort_lib PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(folder_sort_lib PUBLIC cxx_std_20)

# --- Compiler Warnings ---
if(MSVC)
    target_compile_options(folder_sort_lib PRIVATE /W4 /permissive-)
else()
    # GCC/Clang (MSYS2)
    target_compile_options(folder_sort_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --- Linking ---
# Note: spdlog usually depends on fmt. If using system deps, 
# ensure you link properly. FetchContent usually handles the fmt dependency for spdlog automatically.
target_link_libraries(folder_sort_lib PUBLIC
    fmt::fmt
    spdlog::spdlog
    CLI11::CLI11
    yaml-cpp
)

# --- Executable Target ---
add_executable(folder-sort cli/main.cpp)

target_link_libraries(folder-sort PRIVATE folder_sort_lib)

# --- Installation ---
include(GNUInstallDirs)
install(TARGETS folder-sort folder_sort_lib
    EXPORT folder_sort_Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)