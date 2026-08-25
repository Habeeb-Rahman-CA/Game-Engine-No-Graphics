#include "System/AISystem.hpp"
#include "World/World.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Profiler.hpp"
#include "Audio/Audio.hpp"
#include <cmath>

namespace Engine {
namespace System {

void AISystem::update(WorldSystem::World& world, double dt) {
    PROFILE_SCOPE("AISystem::update");

    using namespace EntitySystem;
    using namespace Math;

    // Locate player entity position
    Entity playerEntity = 0;
    Vec3 playerPos;

    for (Entity e : world.get_entities()) {
        std::string name = world.get_name(e);
        if (name == "Player" || name == "Player_Hero") {
            playerEntity = e;
            auto* t = world.get_transform(e);
            if (t) playerPos = t->position;
            break;
        }
    }

    // Process all entities with EnemyAI
    for (Entity e : world.get_entities()) {
        if (!world.has_ai(e)) continue;

        auto* ai        = world.get_ai(e);
        auto* transform = world.get_transform(e);
        auto* velocity  = world.get_velocity(e);
        auto* health    = world.get_health(e);

        if (!ai || !transform || !velocity) continue;

        // Check if dead
        if (health && health->value <= 0 && ai->state != AIState::Dead) {
            ai->state = AIState::Dead;
            velocity->value = Vec3(0.0, 0.0, 0.0);
            LOG_INFO("[AI STATE CHANGE] Entity '" + world.get_name(e) + "' -> DEAD");
            continue;
        }

        if (ai->state == AIState::Dead) continue;

        double distToPlayer = playerEntity ? (playerPos - transform->position).length() : 99999.0;
        AIState previousState = ai->state;

        switch (ai->state) {
            case AIState::Idle:
                velocity->value = Vec3(0.0, 0.0, 0.0);
                ai->stateTimer += dt;
                if (distToPlayer <= ai->detectionRange) {
                    ai->state = AIState::Chase;
                    LOG_INFO("[AI EVENT] 'Player Detected!' -> Transitioning Idle -> Chase");
                } else if (ai->stateTimer >= 2.0) {
                    ai->state = AIState::Patrol;
                    ai->stateTimer = 0.0;
                }
                break;

            case AIState::Patrol: {
                Vec3 target = ai->headingToEnd ? ai->patrolEndPos : ai->patrolStartPos;
                Vec3 dir = target - transform->position;
                double distToTarget = dir.length();

                if (distToTarget <= 0.5) {
                    ai->headingToEnd = !ai->headingToEnd;
                    ai->state = AIState::Idle;
                    ai->stateTimer = 0.0;
                } else {
                    dir.normalize();
                    velocity->value = dir * ai->moveSpeed;
                }

                if (distToPlayer <= ai->detectionRange) {
                    ai->state = AIState::Chase;
                    LOG_INFO("[AI EVENT] 'Player Detected!' -> Transitioning Patrol -> Chase");
                }
                break;
            }

            case AIState::Chase: {
                Vec3 dir = playerPos - transform->position;
                dir.normalize();
                velocity->value = dir * (ai->moveSpeed * 1.4); // Run faster during chase

                if (distToPlayer <= ai->attackRange) {
                    ai->state = AIState::Attack;
                } else if (distToPlayer > ai->detectionRange * 1.5) {
                    ai->state = AIState::Patrol;
                    LOG_INFO("[AI EVENT] 'Player Escaped' -> Transitioning Chase -> Patrol");
                }
                break;
            }

            case AIState::Attack:
                velocity->value = Vec3(0.0, 0.0, 0.0); // Stop to attack
                ai->stateTimer += dt;
                if (ai->stateTimer >= 1.0) {
                    LOG_INFO("[AI ATTACK] Entity '" + world.get_name(e) + "' strikes Player for 15 damage!");
                    AudioSystem::Audio::play_sound("slash.wav");
                    ai->stateTimer = 0.0;
                }

                if (distToPlayer > ai->attackRange) {
                    ai->state = AIState::Chase;
                }
                break;

            case AIState::Dead:
                velocity->value = Vec3(0.0, 0.0, 0.0);
                break;
        }

        if (ai->state != previousState) {
            LOG_INFO("[AI FSM TRANSITION] '" + world.get_name(e) + "': " + 
                     aiStateToString(previousState) + " -> " + aiStateToString(ai->state));
        }
    }
}

} // namespace System
} // namespace Engine
