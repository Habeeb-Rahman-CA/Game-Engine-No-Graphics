#include "System/AnimationSystem.hpp"
#include "World/World.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Profiler.hpp"

namespace Engine {
namespace System {

void AnimationSystem::update(WorldSystem::World& world, double dt) {
    PROFILE_SCOPE("AnimationSystem");

    for (EntitySystem::Entity e : world.get_entities()) {
        auto* animator = world.get_animator(e);
        auto* anim     = world.get_animation(e);
        auto* sprite   = world.get_sprite(e);
        auto* sheet    = world.get_spritesheet(e);

        // 1. Process Animator State Machine Clips
        if (animator && animator->playing) {
            auto it = animator->clips.find(animator->currentClip);
            if (it != animator->clips.end()) {
                const auto& clip = it->second;
                animator->elapsedTime += dt;
                if (animator->elapsedTime >= clip.frameDuration) {
                    animator->elapsedTime = 0.0;
                    animator->currentFrame++;

                    if (animator->currentFrame > clip.endFrame) {
                        if (clip.loop) {
                            animator->currentFrame = clip.startFrame;
                        } else {
                            animator->currentFrame = clip.endFrame;
                            animator->playing = false;
                        }
                    }

                    if (sprite && sheet) {
                        sprite->srcOffset = sheet->getFrameOffset(animator->currentFrame);
                    }
                }
            }
        }
        // 2. Legacy Single Animation Component
        else if (anim && anim->playing) {
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
}

} // namespace System
} // namespace Engine
