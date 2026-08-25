#include "Scene/SceneSerializer.hpp"
#include "Debug/Logger.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace Engine {
namespace SceneSystem {

static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

static std::string extractValue(const std::string& line, const std::string& key) {
    size_t pos = line.find("\"" + key + "\":");
    if (pos == std::string::npos) pos = line.find(key + ":");
    if (pos == std::string::npos) return "";
    size_t colon = line.find(':', pos);
    if (colon == std::string::npos) return "";
    std::string val = line.substr(colon + 1);
    size_t comma = val.find(',');
    if (comma != std::string::npos) val = val.substr(0, comma);
    size_t brace = val.find('}');
    if (brace != std::string::npos) val = val.substr(0, brace);
    val = trim(val);
    if (val.front() == '"' && val.back() == '"') val = val.substr(1, val.length() - 2);
    return trim(val);
}

bool SceneSerializer::load_scene(WorldSystem::World& world, const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open scene file: '" + filePath + "'");
        return false;
    }

    LOG_INFO("Loading scene from file: '" + filePath + "'...");
    std::string line;
    std::string currentEntityName = "";
    EntitySystem::Entity currentEntity = 0;
    bool inEntityBlock = false;

    EntitySystem::Transform transform;
    EntitySystem::Velocity velocity;
    EntitySystem::Health health;
    EntitySystem::Sprite sprite;

    bool hasTransform = false;
    bool hasVelocity = false;
    bool hasHealth = false;
    bool hasSprite = false;

    auto commitEntity = [&]() {
        if (currentEntity != 0) {
            if (hasTransform) world.add_transform(currentEntity, transform);
            if (hasVelocity)  world.add_velocity(currentEntity, velocity);
            if (hasHealth)    world.add_health(currentEntity, health);
            if (hasSprite)    world.add_sprite(currentEntity, sprite);
            LOG_INFO("Loaded Entity '" + currentEntityName + "' (ID: " + std::to_string(currentEntity) + ")");
        }
        currentEntity = 0;
        currentEntityName = "";
        hasTransform = hasVelocity = hasHealth = hasSprite = false;
    };

    while (std::getline(file, line)) {
        std::string trimmed = trim(line);
        if (trimmed.empty()) continue;

        if (trimmed.find("\"name\":") != std::string::npos) {
            if (inEntityBlock) commitEntity();
            currentEntityName = extractValue(trimmed, "name");
            currentEntity = world.create_entity(currentEntityName);
            inEntityBlock = true;
        } else if (trimmed.find("\"transform\":") != std::string::npos || trimmed.find("transform") != std::string::npos) {
            std::string xStr = extractValue(trimmed, "x");
            std::string yStr = extractValue(trimmed, "y");
            std::string zStr = extractValue(trimmed, "z");
            double x = xStr.empty() ? 0.0 : std::stod(xStr);
            double y = yStr.empty() ? 0.0 : std::stod(yStr);
            double z = zStr.empty() ? 0.0 : std::stod(zStr);
            transform = EntitySystem::Transform(Math::Vec3(x, y, z));
            hasTransform = true;
        } else if (trimmed.find("\"velocity\":") != std::string::npos || trimmed.find("velocity") != std::string::npos) {
            std::string vxStr = extractValue(trimmed, "vx");
            if (vxStr.empty()) vxStr = extractValue(trimmed, "x");
            std::string vyStr = extractValue(trimmed, "vy");
            if (vyStr.empty()) vyStr = extractValue(trimmed, "y");
            double vx = vxStr.empty() ? 0.0 : std::stod(vxStr);
            double vy = vyStr.empty() ? 0.0 : std::stod(vyStr);
            velocity = EntitySystem::Velocity(Math::Vec3(vx, vy, 0.0));
            hasVelocity = true;
        } else if (trimmed.find("\"health\":") != std::string::npos || trimmed.find("health") != std::string::npos) {
            std::string curStr = extractValue(trimmed, "current");
            std::string maxStr = extractValue(trimmed, "max");
            int cur = curStr.empty() ? 100 : std::stoi(curStr);
            int max = maxStr.empty() ? 100 : std::stoi(maxStr);
            health = EntitySystem::Health(cur, max);
            hasHealth = true;
        } else if (trimmed.find("\"sprite\":") != std::string::npos || trimmed.find("sprite") != std::string::npos) {
            std::string tex = extractValue(trimmed, "textureId");
            if (tex.empty()) tex = "default_sprite";
            std::string wStr = extractValue(trimmed, "width");
            std::string hStr = extractValue(trimmed, "height");
            double w = wStr.empty() ? 4.0 : std::stod(wStr);
            double h = hStr.empty() ? 4.0 : std::stod(hStr);
            sprite = EntitySystem::Sprite(tex, Math::Vec2(w, h));
            hasSprite = true;
        }
    }

    if (inEntityBlock) commitEntity();

    LOG_INFO("Successfully loaded scene file: '" + filePath + "'");
    return true;
}

bool SceneSerializer::save_scene(const WorldSystem::World& world, const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open scene file for writing: '" + filePath + "'");
        return false;
    }

    file << "{\n";
    file << "  \"scene\": \"Engine Saved Scene\",\n";
    file << "  \"entities\": [\n";

    const auto& entities = world.get_entities();
    for (size_t i = 0; i < entities.size(); ++i) {
        EntitySystem::Entity e = entities[i];
        file << "    {\n";
        file << "      \"name\": \"" << world.get_name(e) << "\"";

        WorldSystem::World& wRef = const_cast<WorldSystem::World&>(world);
        auto* transform = wRef.get_transform(e);
        auto* velocity  = wRef.get_velocity(e);
        auto* health    = wRef.get_health(e);
        auto* sprite    = wRef.get_sprite(e);

        if (transform) {
            file << ",\n      \"transform\": { \"x\": " << transform->position.x 
                 << ", \"y\": " << transform->position.y 
                 << ", \"z\": " << transform->position.z << " }";
        }
        if (velocity) {
            file << ",\n      \"velocity\": { \"x\": " << velocity->value.x 
                 << ", \"y\": " << velocity->value.y << " }";
        }
        if (health) {
            file << ",\n      \"health\": { \"current\": " << health->value 
                 << ", \"max\": " << health->maxHp << " }";
        }
        if (sprite) {
            file << ",\n      \"sprite\": { \"textureId\": \"" << sprite->textureId 
                 << "\", \"width\": " << sprite->size.x 
                 << ", \"height\": " << sprite->size.y << " }";
        }

        file << "\n    }" << (i + 1 < entities.size() ? "," : "") << "\n";
    }

    file << "  ]\n";
    file << "}\n";

    LOG_INFO("Successfully saved scene to file: '" + filePath + "'");
    return true;
}

} // namespace SceneSystem
} // namespace Engine
