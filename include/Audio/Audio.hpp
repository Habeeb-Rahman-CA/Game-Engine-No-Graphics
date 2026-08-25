#ifndef AUDIO_AUDIO_HPP
#define AUDIO_AUDIO_HPP

#include <string>
#include <unordered_map>

namespace Engine {
namespace AudioSystem {

struct Sound {
    std::string id;
    std::string filePath;
    float volume = 1.0f;
};

struct Music {
    std::string id;
    std::string filePath;
    float volume = 1.0f;
    bool isLooping = true;
    bool isPlaying = false;
};

class Audio {
public:
    static bool init();
    static void play_sound(const std::string& soundFile, float volume = 1.0f);
    static void play_music(const std::string& musicFile, float volume = 1.0f, bool loop = true);
    static void stop_music();
    static void set_master_volume(float volume);
    static float get_master_volume();
    static void shutdown();

private:
    static float s_masterVolume;
    static std::string s_currentMusicTrack;
    static std::unordered_map<std::string, Sound> s_soundCache;
    static std::unordered_map<std::string, Music> s_musicCache;
};

} // namespace AudioSystem
} // namespace Engine

#endif // AUDIO_AUDIO_HPP
