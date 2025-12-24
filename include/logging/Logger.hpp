// include/logging/Logger.hpp

#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace logging {

    enum class LogLevel {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    class Logger {
        public:
            static void Init(
                const std::string& logFilePath,
                LogLevel level = LogLevel::Info,
                bool logToConsole = true
            );

            static void Shutdown();


        static void Trace(const std::string& msg);
        static void Debug(const std::string& msg);
        static void Info(const std::string& msg);
        static void Warn(const std::string& msg);
        static void Error(const std::string& msg);
        static void Critical(const std::string& msg);
    };

}