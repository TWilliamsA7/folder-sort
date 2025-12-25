#include "logging/Logger.hpp"

#include <filesystem>
#include <stdexcept>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace logging {

    namespace {
        std::vector<spdlog::sink_ptr> g_sinks;
        spdlog::level::level_enum g_level = spdlog::level::info;
        bool g_initialized = false;
    }


    void Logger::Init(const std::string& logDir) {
        if (g_initialized) return;

        std::filesystem::create_directories(logDir);
        g_sinks.clear();

        auto consoleSink =
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_pattern("[%T] [%n] [%^%l%$] %v");

        auto fileSink =
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                logDir + "/app.log", true);
        fileSink->set_pattern("[%Y-%m-%d %T] [%n] [%l] %v");

        g_sinks = { consoleSink, fileSink };
        g_level = spdlog::level::debug;

        g_initialized = true;
    }

    void Logger::InitForTests(const std::string& logDir) {
        if (g_initialized) return;

        std::filesystem::create_directories(logDir);
        g_sinks.clear();

        auto fileSink =
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                logDir + "/test.log", true);
        fileSink->set_pattern("[%n] [%l] %v");

        g_sinks = { fileSink };
        g_level = spdlog::level::trace;

        g_initialized = true;
    }

    std::shared_ptr<spdlog::logger> Logger::Get(const std::string& name) {
        if (!g_initialized) {
            throw std::runtime_error("Logger::Init must be called first");
        }

        if (auto existing = spdlog::get(name)) {
            return existing;
        }

        auto logger = std::make_shared<spdlog::logger>(
            name, g_sinks.begin(), g_sinks.end());

        logger->set_level(g_level);
        logger->flush_on(spdlog::level::trace);

        spdlog::register_logger(logger);
        return logger;
    }


    void Logger::Shutdown() {
        spdlog::drop_all();  
        g_sinks.clear();
        g_initialized = false;
    }

} // namespace logging
