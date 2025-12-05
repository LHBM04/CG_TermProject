#pragma once

#include <filesystem>
#include <span>

/**
 * @namespace Path
 * 
 * @brief 경로 관련 기능을 정의합니다.
 */
namespace Path
{
    /**
     * @brief 지정한 경로들을 합쳐 최종 경로를 반환합니다.
     * 
     * @param paths_ 지정할 경로들
     * 
     * @return std::filesystem::path_ 최종 경로
     */
    std::filesystem::path Combine(std::span<const std::filesystem::path> paths_);

    /**
     * @brief 두 경로를 합쳐 최종 경로를 반환합니다.
     * 
     * @param first_  지정할 첫 번째 경로
     * @param second_ 지정할 두 번째 경로
     * 
     * @return std::filesystem::path 최종 경로
     */
    std::filesystem::path Combine(const std::filesystem::path& first_, 
                                  const std::filesystem::path& second_);

    /**
     * @brief 지정한 경로 내 위치한 파일 이름(확장자 포함)을 반환합니다.
     * 
     * @param path_ 지정할 경로
     * 
     * @return std::filesyste::path 파일 이름(확장자 포함)
     */
    std::filesystem::path GetFileName(const std::filesystem::path& path_);

    /**
     * @brief 지정한 경로 내 위치한 파일 이름(확장자 제외)을 반환합니다.
     * 
     * @param path 지정할 경로
     * 
     * @return std::string 파일 이름(확장자 제외)
     */
    std::string GetFileNameWithoutExtension(const std::filesystem::path& path);

    /**
     * @brief 지정한 경로 내 위치한 파일의 확장자(점 포함)를 반환합니다.
     * 
     * @param path 지정할 경로
     * 
     * @return std::string 파일 확장자(점 포함)
     */
    std::string GetExtension(const std::filesystem::path& path);

    /**
     * @brief 지정한 경로 내 위치한 상위 디렉토리 경로를 반환합니다.
     * 
     * @param path 지정할 경로
     * 
     * @return std::filesystem::path 상위 디렉토리 경로
     */
    std::filesystem::path GetDirectoryName(const std::filesystem::path& path);

    /**
     * @brief 지정한 경로를 절대 경로로 변환하여 반환합니다.
     * 
     * @param path 지정할 경로
     * 
     * @return std::filesystem::path 절대 경로
     */
    std::filesystem::path GetFullPath(const std::filesystem::path& path);
} // namespace Path