#ifndef PLATFORM_FILESYSTEM_HPP
#define PLATFORM_FILESYSTEM_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace Engine {
namespace Platform {

class FileSystem {
public:
    static bool exists(const std::string& path);
    static std::string readFileText(const std::string& path);
    static bool writeFileText(const std::string& path, const std::string& content);
    static std::vector<uint8_t> readFileBinary(const std::string& path);
    static bool writeFileBinary(const std::string& path, const std::vector<uint8_t>& data);
    static std::string getWorkingDirectory();
};

} // namespace Platform
} // namespace Engine

#endif // PLATFORM_FILESYSTEM_HPP
