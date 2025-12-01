#pragma once

#include <string>
#include <vector>
#include "Common.h" // filesystem 헤더가 포함되어 있다고 가정합니다.

// 네임스페이스가 겹칠 수 있으므로 namespace 사용 시 주의가 필요하지만,
// 기존 구조를 유지하며 정리했습니다.

namespace Path
{
    /**
     * @brief 여러 경로를 결합합니다.
     * @return 결합된 새로운 경로 객체 (값 반환)
     */
    std::filesystem::path Combine(const std::vector<std::filesystem::path>& paths);

    /**
     * @brief 두 경로를 결합합니다.
     */
    std::filesystem::path Combine(const std::filesystem::path& path1, const std::filesystem::path& path2);

    /**
     * @brief 경로에서 파일 이름(확장자 포함)을 반환합니다.
     */
    std::filesystem::path GetFileName(const std::filesystem::path& path);

    /**
     * @brief 경로에서 확장자를 제외한 파일 이름을 반환합니다.
     */
    std::string GetFileNameWithoutExtension(const std::filesystem::path& path);

    /**
     * @brief 경로에서 확장자(점 포함)를 반환합니다.
     */
    std::string GetExtension(const std::filesystem::path& path);

    /**
     * @brief 상위 디렉토리 경로를 반환합니다.
     */
    std::filesystem::path GetDirectoryName(const std::filesystem::path& path);

    /**
     * @brief 절대 경로로 변환하여 반환합니다.
     */
    std::filesystem::path GetFullPath(const std::filesystem::path& path);
} // namespace Path

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

namespace Directory
{
    bool Exists(const std::filesystem::path& path);

    void Create(const std::filesystem::path& path);

    void Delete(const std::filesystem::path& path, bool recursive = false);

    void Move(const std::filesystem::path& source, const std::filesystem::path& destination);

    std::filesystem::path GetCurrentDirectory();

    /**
     * @brief 디렉토리 내 파일 목록을 반환합니다.
     * @return path 객체의 벡터 (참조가 아닌 값이어야 안전함)
     */
    std::vector<std::filesystem::path>
            GetFiles(const std::filesystem::path& path, std::string_view searchPattern = "*", bool recursive = false);

    std::vector<std::filesystem::path> GetDirectories(const std::filesystem::path& path, bool recursive = false);
} // namespace Directory