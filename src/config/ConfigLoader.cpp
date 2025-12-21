// src/config/ConfigLoader.cpp

#include "config/ConfigLoader.hpp"

ConfigLoader::ConfigLoader(const std::string& path) : path_(path) {}

YAML::Node ConfigLoader::load() const {
    YAML::Node root = YAML::LoadFile(path_);

    if (!root["rules"] || !root["rules"].IsSequence()) {
        throw std::runtime_error("Config must contain a 'rules' sequence");
    }

    return root;
}