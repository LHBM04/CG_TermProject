#pragma once

#include <filesystem>
#include <vector>

/**
 * @namespace File
 * 
 * @brief 파일 관련 기능을 정의합니다.
 */
namespace File
{
    bool Exists(const std::filesystem::path& path);

    void Delete(const std::filesystem::path& path);

    void Copy(const std::filesystem::path& source, const std::filesystem::path& destination, bool overwrite = false);

    void Move(const std::filesystem::path& source, const std::filesystem::path& destination);

    std::string ReadAllText(const std::filesystem::path& path);

    void WriteAllText(const std::filesystem::path& path, std::string_view contents);

    void AppendAllText(const std::filesystem::path& path, std::string_view contents);

    std::vector<unsigned char> ReadAllBytes(const std::filesystem::path& path);

    void WriteAllBytes(const std::filesystem::path& path, const std::vector<unsigned char>& bytes);
} // namespace File