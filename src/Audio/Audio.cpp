#include "Audio/Audio.hpp"
#include "Debug/Logger.hpp"

namespace Engine {
namespace AudioSystem {

float Audio::s_masterVolume = 1.0f;
std::string Audio::s_currentMusicTrack = "";
std::unordered_map<std::string, Sound> Audio::s_soundCache;
std::unordered_map<std::string, Music> Audio::s_musicCache;

bool Audio::init() {
    s_soundCache.clear();
    s_musicCache.clear();
    s_masterVolume = 1.0f;
    s_currentMusicTrack = "";
    LOG_INFO("Audio Manager Subsystem initialized successfully.");
    return true;
}

void Audio::play_sound(const std::string& soundFile, float volume) {
    float effectiveVol = volume * s_masterVolume;
    s_soundCache[soundFile] = { soundFile, soundFile, effectiveVol };
    LOG_INFO("[AUDIO SFX] Playing Sound Effect: '" + soundFile + "' (Volume: " + std::to_string(effectiveVol) + ")");
}

void Audio::play_music(const std::string& musicFile, float volume, bool loop) {
    if (!s_currentMusicTrack.empty() && s_currentMusicTrack != musicFile) {
        LOG_INFO("[AUDIO MUSIC] Stopping track: '" + s_currentMusicTrack + "'");
    }

    float effectiveVol = volume * s_masterVolume;
    s_currentMusicTrack = musicFile;
    s_musicCache[musicFile] = { musicFile, musicFile, effectiveVol, loop, true };
    LOG_INFO("[AUDIO MUSIC] Playing Background Music: '" + musicFile + "' (Volume: " + std::to_string(effectiveVol) + ", Loop: " + (loop ? "True" : "False") + ")");
}

void Audio::stop_music() {
    if (!s_currentMusicTrack.empty()) {
        LOG_INFO("[AUDIO MUSIC] Stopped track: '" + s_currentMusicTrack + "'");
        s_musicCache[s_currentMusicTrack].isPlaying = false;
        s_currentMusicTrack = "";
    }
}

void Audio::set_master_volume(float volume) {
    s_masterVolume = (volume < 0.0f) ? 0.0f : ((volume > 1.0f) ? 1.0f : volume);
    LOG_INFO("[AUDIO MASTER] Master volume set to: " + std::to_string(s_masterVolume));
}

float Audio::get_master_volume() {
    return s_masterVolume;
}

void Audio::shutdown() {
    stop_music();
    s_soundCache.clear();
    s_musicCache.clear();
    LOG_INFO("Audio Manager Subsystem shutdown.");
}

} // namespace AudioSystem
} // namespace Engine
