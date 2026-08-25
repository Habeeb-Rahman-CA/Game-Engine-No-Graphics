#include "Resources/Resource.hpp"
#include "Debug/Logger.hpp"
#include <fstream>
#include <sstream>

namespace Engine {
namespace Resources {

// --- TextResource Implementation ---
bool TextResource::loadFromFile() {
    std::ifstream file(m_filepath);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open text resource file: " + m_filepath);
        return false;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    m_content = buffer.str();
    m_isLoaded = true;
    return true;
}

// --- GameDataResource Implementation ---
bool GameDataResource::loadFromFile() {
    std::ifstream file(m_filepath);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open game data resource file: " + m_filepath);
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t delim = line.find('=');
        if (delim != std::string::npos) {
            std::string key = line.substr(0, delim);
            std::string val = line.substr(delim + 1);
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            val.erase(0, val.find_first_not_of(" \t"));
            val.erase(val.find_last_not_of(" \t") + 1);
            m_properties[key] = val;
        }
    }
    m_isLoaded = true;
    return true;
}

std::string GameDataResource::getValue(const std::string& key, const std::string& defaultVal) const {
    auto it = m_properties.find(key);
    return (it != m_properties.end()) ? it->second : defaultVal;
}

// --- LevelDataResource Implementation ---
bool LevelDataResource::loadFromFile() {
    std::ifstream file(m_filepath);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open level resource file: " + m_filepath);
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t delim = line.find('=');
        if (delim != std::string::npos) {
            std::string key = line.substr(0, delim);
            std::string val = line.substr(delim + 1);

            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t\r\n"));
            key.erase(key.find_last_not_of(" \t\r\n") + 1);
            val.erase(0, val.find_first_not_of(" \t\r\n"));
            val.erase(val.find_last_not_of(" \t\r\n") + 1);

            if (key == "level_name") m_levelName = val;
            else if (key == "difficulty") m_difficulty = std::stoi(val);
            else if (key == "entities") {
                std::stringstream ss(val);
                std::string entity;
                while (std::getline(ss, entity, ',')) {
                    entity.erase(0, entity.find_first_not_of(" \t\r\n"));
                    entity.erase(entity.find_last_not_of(" \t\r\n") + 1);
                    m_initialEntities.push_back(entity);
                }
            }
        }
    }
    m_isLoaded = true;
    return true;
}

// --- BinaryResource Implementation ---
bool BinaryResource::loadFromFile() {
    std::ifstream file(m_filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open binary resource file: " + m_filepath);
        return false;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    m_data.resize(size);
    if (file.read(reinterpret_cast<char*>(m_data.data()), size)) {
        m_isLoaded = true;
        return true;
    }
    return false;
}

} // namespace Resources
} // namespace Engine
