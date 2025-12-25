// include/logging/Logger.hpp

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace logging {

    class Logger {
        public:
            // Production init
            static void Init(const std::string& logDir);

            // Test init (file only, immediate flush)
            static void InitForTests(const std::string& logDir);

            static void Shutdown();

            // Retrieve or create a named logger
            static std::shared_ptr<spdlog::logger> Get(const std::string& name);
    };

}
