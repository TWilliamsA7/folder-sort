// src/config/ConfigLoader.cpp

#include "config/ConfigLoader.hpp"

namespace {
    const char *kLoggerName = "app.config.loader";
}

ConfigLoader::ConfigLoader(const std::string& path) : path_(path) {
    auto log = logging::Logger::Get(kLoggerName);
}

YAML::Node ConfigLoader::load() const {
    YAML::Node root = YAML::LoadFile(path_);
    auto log = logging::Logger::Get(kLoggerName);

    if (!root["rules"] || !root["rules"].IsSequence()) {
        log->error("Config File {} does not contain a 'rules' sequence", path_);
        throw std::runtime_error("Config must contain a 'rules' sequence");
    }

    log->info("Successfully loaded rule configuration from {}", path_);
    return root;
}