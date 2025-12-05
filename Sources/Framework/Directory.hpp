#pragma once

#include <vector>
#include <filesystem>

/**
 * @brief 디렉터리(폴더) 관련 기능을 정의합니다.
 */
namespace Directory
{
    [[nodiscard]]
    bool Exists(const std::filesystem::path& path);

    void Create(const std::filesystem::path& path);

    void Delete(const std::filesystem::path& path, bool recursive = false);

    void Move(const std::filesystem::path& source, const std::filesystem::path& destination);

    std::filesystem::path GetCurrentDirectory();

    std::vector<std::filesystem::path> GetFiles(const std::filesystem::path& path, std::string_view searchPattern = "*", bool recursive = false);

    std::vector<std::filesystem::path> GetDirectories(const std::filesystem::path& path, bool recursive = false);
} // namespace Directory