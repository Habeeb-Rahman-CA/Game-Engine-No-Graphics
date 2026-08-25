#ifndef SCENE_SCENESERIALIZER_HPP
#define SCENE_SCENESERIALIZER_HPP

#include "World/World.hpp"
#include <string>

namespace Engine {
namespace SceneSystem {

class SceneSerializer {
public:
    static bool load_scene(WorldSystem::World& world, const std::string& filePath);
    static bool save_scene(const WorldSystem::World& world, const std::string& filePath);
};

} // namespace SceneSystem
} // namespace Engine

#endif // SCENE_SCENESERIALIZER_HPP
