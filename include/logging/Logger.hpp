// include/logging/Logger.hpp

#pragma once

#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/chrono.h>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <chrono>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <stdexcept>

namespace logging {

    class Logger {
        public:

            // Production init
            static std::filesystem::path Init(const std::filesystem::path& logDir, const bool use_console);

            // Test init (file only, immediate flush)
            static std::filesystem::path InitForTests(const std::filesystem::path& logDir, const bool use_console);

            static void Shutdown();

            // Retrieve or create a named logger
            static std::shared_ptr<spdlog::logger> Get(const std::string& name);

        private:
            static std::filesystem::path InitInternal(const std::filesystem::path& logDir, const std::string& prefix, spdlog::level::level_enum level, bool includeConsole);
    };

}
