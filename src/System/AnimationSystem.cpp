#include "System/AnimationSystem.hpp"
#include "World/World.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Profiler.hpp"

namespace Engine {
namespace System {

void AnimationSystem::update(WorldSystem::World& world, double dt) {
    PROFILE_SCOPE("AnimationSystem");

    for (EntitySystem::Entity e : world.get_entities()) {
        auto* anim   = world.get_animation(e);
        auto* sprite = world.get_sprite(e);
        auto* sheet  = world.get_spritesheet(e);

        if (!anim || !anim->playing) continue;

        anim->elapsedTime += dt;
        if (anim->elapsedTime >= anim->frameDuration) {
            anim->elapsedTime = 0.0;
            anim->currentFrame++;

            if (anim->currentFrame > anim->endFrame) {
                if (anim->loop) {
                    anim->currentFrame = anim->startFrame;
                } else {
                    anim->currentFrame = anim->endFrame;
                    anim->playing = false;
                }
            }

            if (sprite && sheet) {
                sprite->srcOffset = sheet->getFrameOffset(anim->currentFrame);
            }
        }
    }
}

} // namespace System
} // namespace Engine
