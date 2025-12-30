// include/config/ConfigLoader.hpp

#pragma once

#include <string>
#include <stdexcept>
#include <yaml-cpp/yaml.h>
#include "logging/Logger.hpp"

class ConfigLoader {
    public:
        // * Initialize Loader pointed at YAML file at path
        explicit ConfigLoader(const std::string& path);

        // * Load YAML file located at path_
        YAML::Node load() const;

    private:
        std::string path_;
};