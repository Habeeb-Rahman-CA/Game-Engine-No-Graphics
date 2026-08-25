#include "Platform/FileSystem.hpp"
#include "Debug/Logger.hpp"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>

namespace Engine {
namespace Platform {

bool FileSystem::exists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

std::string FileSystem::readFileText(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        LOG_ERROR("FileSystem: Failed to open file '" + path + "' for reading.");
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

bool FileSystem::writeFileText(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) {
        LOG_ERROR("FileSystem: Failed to open file '" + path + "' for writing.");
        return false;
    }
    file << content;
    return true;
}

std::vector<uint8_t> FileSystem::readFileBinary(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        LOG_ERROR("FileSystem: Failed to open binary file '" + path + "' for reading.");
        return {};
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        return buffer;
    }
    return {};
}

bool FileSystem::writeFileBinary(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        LOG_ERROR("FileSystem: Failed to open binary file '" + path + "' for writing.");
        return false;
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return true;
}

std::string FileSystem::getWorkingDirectory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        return std::string(cwd);
    }
    return "";
}

} // namespace Platform
} // namespace Engine
