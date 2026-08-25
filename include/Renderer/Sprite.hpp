#ifndef RENDERER_SPRITE_HPP
#define RENDERER_SPRITE_HPP

#include "Math/Vec2.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace Engine {
namespace RenderSystem {

using Vec2 = Math::Vec2;

struct Texture {
    std::string id;
    int width = 0;
    int height = 0;
    std::vector<char> pixelData;

    Texture() = default;
    Texture(const std::string& texUrl, int w, int h)
        : id(texUrl), width(w), height(h) {}
};

struct Sprite {
    std::string textureId = "default";
    Vec2 size = Vec2(4.0, 2.0);       // World rendering dimensions
    Vec2 srcOffset = Vec2(0.0, 0.0);  // Frame top-left in sheet
    Vec2 srcSize = Vec2(1.0, 1.0);    // Frame width/height in sheet

    Sprite() = default;
    Sprite(const std::string& tex, Vec2 s = Vec2(4.0, 2.0))
        : textureId(tex), size(s) {}
};

struct SpriteSheet {
    std::string textureId;
    int frameWidth = 16;
    int frameHeight = 16;
    int columns = 4;
    int rows = 4;

    SpriteSheet() = default;
    SpriteSheet(const std::string& tex, int fw, int fh, int cols, int r)
        : textureId(tex), frameWidth(fw), frameHeight(fh), columns(cols), rows(r) {}

    Vec2 getFrameOffset(int frameIndex) const {
        int col = frameIndex % columns;
        int row = frameIndex / columns;
        return Vec2(col * frameWidth, row * frameHeight);
    }
};

struct Animation {
    int startFrame = 0;
    int endFrame = 3;
    int currentFrame = 0;
    double frameDuration = 0.1; // 100ms per frame
    double elapsedTime = 0.0;
    bool loop = true;
    bool playing = true;

    Animation() = default;
    Animation(int start, int end, double duration = 0.1, bool isLooping = true)
        : startFrame(start), endFrame(end), currentFrame(start),
          frameDuration(duration), loop(isLooping), playing(true) {}
};

struct AnimationClip {
    std::string name;
    int startFrame = 0;
    int endFrame = 0;
    double frameDuration = 0.1;
    bool loop = true;

    AnimationClip() = default;
    AnimationClip(const std::string& n, int start, int end, double duration = 0.1, bool isLooping = true)
        : name(n), startFrame(start), endFrame(end), frameDuration(duration), loop(isLooping) {}
};

struct Animator {
    std::unordered_map<std::string, AnimationClip> clips;
    std::string currentClip = "idle";
    int currentFrame = 0;
    double elapsedTime = 0.0;
    bool playing = true;

    void addClip(const AnimationClip& clip) {
        clips[clip.name] = clip;
        if (currentClip.empty()) {
            currentClip = clip.name;
            currentFrame = clip.startFrame;
        }
    }

    bool play(const std::string& clipName) {
        if (currentClip == clipName) return true;
        auto it = clips.find(clipName);
        if (it != clips.end()) {
            currentClip = clipName;
            currentFrame = it->second.startFrame;
            elapsedTime = 0.0;
            playing = true;
            return true;
        }
        return false;
    }
};

} // namespace RenderSystem
} // namespace Engine

#endif // RENDERER_SPRITE_HPP
