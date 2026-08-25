#ifndef RESOURCES_ASSETMANAGER_HPP
#define RESOURCES_ASSETMANAGER_HPP

#include "Resources/Resource.hpp"
#include "Debug/Logger.hpp"
#include <unordered_map>
#include <memory>
#include <string>
#include <type_traits>

namespace Engine {
namespace Resources {

class AssetManager {
public:
    AssetManager() = default;
    ~AssetManager() = default;

    // Disable copy
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    template <typename T>
    std::shared_ptr<T> load(const std::string& filepath) {
        static_assert(std::is_base_of<IResource, T>::value, "T must derive from IResource");

        auto it = m_cache.find(filepath);
        if (it != m_cache.end()) {
            LOG_INFO("[AssetManager] CACHE HIT: '" + filepath + "'");
            return std::static_pointer_cast<T>(it->second);
        }

        LOG_INFO("[AssetManager] CACHE MISS: Loading asset from disk: '" + filepath + "'...");
        auto resource = std::make_shared<T>(filepath);
        if (resource->loadFromFile()) {
            m_cache[filepath] = resource;
            LOG_INFO("[AssetManager] Loaded and cached resource: '" + filepath + "'");
            return resource;
        }

        LOG_ERROR("[AssetManager] Failed to load asset: '" + filepath + "'");
        return nullptr;
    }

    bool isLoaded(const std::string& filepath) const {
        return m_cache.find(filepath) != m_cache.end();
    }

    void unload(const std::string& filepath) {
        auto it = m_cache.find(filepath);
        if (it != m_cache.end()) {
            LOG_INFO("[AssetManager] Unloaded asset: '" + filepath + "'");
            m_cache.erase(it);
        }
    }

    void clearCache() {
        LOG_INFO("[AssetManager] Clearing asset cache (" + std::to_string(m_cache.size()) + " items)");
        m_cache.clear();
    }

    size_t getCachedAssetCount() const { return m_cache.size(); }

private:
    std::unordered_map<std::string, std::shared_ptr<IResource>> m_cache;
};

} // namespace Resources
} // namespace Engine

#endif // RESOURCES_ASSETMANAGER_HPP
