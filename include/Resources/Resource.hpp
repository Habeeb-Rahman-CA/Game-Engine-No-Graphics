#ifndef RESOURCES_RESOURCE_HPP
#define RESOURCES_RESOURCE_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cstdint>

namespace Engine {
namespace Resources {

class IResource {
public:
    explicit IResource(const std::string& filepath) : m_filepath(filepath), m_isLoaded(false) {}
    virtual ~IResource() = default;

    virtual bool loadFromFile() = 0;

    const std::string& getFilePath() const { return m_filepath; }
    bool isLoaded() const { return m_isLoaded; }

protected:
    std::string m_filepath;
    bool m_isLoaded;
};

// 1. Text File Asset Resource
class TextResource : public IResource {
public:
    explicit TextResource(const std::string& filepath) : IResource(filepath) {}
    bool loadFromFile() override;

    const std::string& getContent() const { return m_content; }

private:
    std::string m_content;
};

// 2. Key-Value Game Data Config Asset
class GameDataResource : public IResource {
public:
    explicit GameDataResource(const std::string& filepath) : IResource(filepath) {}
    bool loadFromFile() override;

    std::string getValue(const std::string& key, const std::string& defaultVal = "") const;
    const std::unordered_map<std::string, std::string>& getAllProperties() const { return m_properties; }

private:
    std::unordered_map<std::string, std::string> m_properties;
};

// 3. Level Data Asset
class LevelDataResource : public IResource {
public:
    explicit LevelDataResource(const std::string& filepath) : IResource(filepath) {}
    bool loadFromFile() override;

    const std::string& getLevelName() const { return m_levelName; }
    int getDifficulty() const { return m_difficulty; }
    const std::vector<std::string>& getInitialEntities() const { return m_initialEntities; }

private:
    std::string m_levelName = "Unknown Level";
    int m_difficulty = 1;
    std::vector<std::string> m_initialEntities;
};

// 4. Binary Asset Resource
class BinaryResource : public IResource {
public:
    explicit BinaryResource(const std::string& filepath) : IResource(filepath) {}
    bool loadFromFile() override;

    const std::vector<uint8_t>& getData() const { return m_data; }
    size_t getSize() const { return m_data.size(); }

private:
    std::vector<uint8_t> m_data;
};

} // namespace Resources
} // namespace Engine

#endif // RESOURCES_RESOURCE_HPP
