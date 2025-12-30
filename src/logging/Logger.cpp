#include "logging/Logger.hpp"

namespace logging {

    namespace {
        std::vector<spdlog::sink_ptr> g_sinks;
        spdlog::level::level_enum g_level = spdlog::level::info;
        bool g_initialized = false;
    }


    std::filesystem::path Logger::InitInternal(
        const std::filesystem::path& logDir, 
        const std::string& prefix, 
        spdlog::level::level_enum level, 
        bool includeConsole
    ) {
        // 1. Fix: Early return must return a value (an empty path or the current log path)
        if (g_initialized) return {}; 

        g_sinks.clear();
        std::filesystem::path logPath;

        if (!logDir.empty()) {
            std::filesystem::create_directories(logDir);
    
            // 2. Generate Filename
            auto now = std::chrono::system_clock::now();
            // Use %H-%M-%S because colons (:) are illegal in Windows filenames
            std::string filename = fmt::format("{}_{:%Y-%m-%d_%H-%M-%S}.log", prefix, now);
            logPath = logDir / filename;
    
            // 3. Setup File Sink
            auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath.string(), true);
            fileSink->set_pattern(prefix == "test" ? "[%n] [%l] %v" : "[%Y-%m-%d %T] [%n] [%l] %v");
            g_sinks.push_back(fileSink);
        }


        // 4. Setup Console Sink (Optional)
        if (includeConsole) {
            auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            consoleSink->set_pattern("[%T] [%n] [%^%l%$] %v");
            g_sinks.push_back(consoleSink);
        }

        g_level = level;
        g_initialized = true;
        
        return logPath;
    }

    std::filesystem::path Logger::Init(const std::filesystem::path& logDir, const bool use_console) {
        return InitInternal(logDir, "folder-sort", spdlog::level::debug, use_console);
    }

    std::filesystem::path Logger::InitForTests(const std::filesystem::path& logDir, const bool use_console) {
        return InitInternal(logDir, "test", spdlog::level::trace, use_console);
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
