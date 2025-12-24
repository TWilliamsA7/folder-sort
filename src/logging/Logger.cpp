// src/logging/Logger.cpp

#include "logging/Logger.hpp"

namespace logging {
    
    static std::shared_ptr<spdlog::logger> s_logger;
    
    static spdlog::level::level_enum ToSpdLevel(LogLevel level) {
        switch (level) {
            case LogLevel::Trace: return spdlog::level::trace;
            case LogLevel::Debug: return spdlog::level::debug;
            case LogLevel::Info: return spdlog::level::info;
            case LogLevel::Warn: return spdlog::level::warn;
            case LogLevel::Error: return spdlog::level::err;
            case LogLevel::Critical: return spdlog::level::critical;
            default: return spdlog::level::info;
        }
    }

    void Logger::Init(const std::string& logFilePath, LogLevel level, bool logToConsole) {
        std::vector<spdlog::sink_ptr> sinks;

        if (logToConsole) {
            auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            consoleSink->set_pattern("[%T] [%^%l%$] %v");
            sinks.push_back(consoleSink);
        }

        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);
        fileSink->set_pattern("[%Y-%m-%d %T] [%l] %v");
        sinks.push_back(fileSink);

        s_logger = std::make_shared<spdlog::logger>("app", sinks.begin(), sinks.end());
        s_logger->set_level(ToSpdLevel(level));
        s_logger->flush_on(spdlog::level::warn);

        spdlog::register_logger(s_logger);
    }

    void Logger::InitForTests(const std::string& logFilePath) {
        std::vector<spdlog::sink_ptr> sinks;

        auto fileSink =
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);

        fileSink->set_pattern("[%Y-%m-%d %T] [%l] %v");
        sinks.push_back(fileSink);

        s_logger = std::make_shared<spdlog::logger>(
            "test_logger", sinks.begin(), sinks.end());

        s_logger->set_level(spdlog::level::trace);
        s_logger->flush_on(spdlog::level::trace);

        spdlog::register_logger(s_logger);
    }

    void Logger::Shutdown() {
        if (s_logger) {
            s_logger->flush();
            spdlog::drop(s_logger->name());
            s_logger.reset();
        }

        spdlog::shutdown();
        spdlog::drop_all();
    }

    void Logger::Trace(const std::string& msg) { s_logger->trace(msg); }
    void Logger::Debug(const std::string& msg) { s_logger->debug(msg); }
    void Logger::Info(const std::string& msg)  { s_logger->info(msg); }
    void Logger::Warn(const std::string& msg)  { s_logger->warn(msg); }
    void Logger::Error(const std::string& msg) { s_logger->error(msg); }
    void Logger::Critical(const std::string& msg) { s_logger->critical(msg); }

}