#pragma once

#include "Common.h"

/**
 * @class Path
 * * @brief 파일 경로 문자열을 조작하는 헬퍼 클래스입니다 (C# System.IO.Path 모방)
 */
class Path final
{
    STATIC_CLASS(Path)

public:
    /**
     * @brief 여러 문자열을 결합하여 하나의 경로로 만듭니다.
     */
    static String Combine(const List<String>& paths) noexcept;

    /**
     * @brief 
     * 
     * @param path1 
     * @param path2 
     * 
     * @return 
     */
    static String Combine(StringView path1, StringView path2) noexcept;

    /**
     * @brief 경로에서 파일 이름(확장자 포함)을 반환합니다.
     */
    static String GetFileName(StringView path) noexcept;

    /**
     * @brief 경로에서 확장자를 제외한 파일 이름을 반환합니다.
     */
    static String GetFileNameWithoutExtension(StringView path) noexcept;

    /**
     * @brief 경로에서 확장자(점 포함)를 반환합니다.
     */
    static String GetExtension(StringView path) noexcept;

    /**
     * @brief 특정 파일의 부모 디렉토리 경로를 반환합니다.
     */
    static String GetDirectoryName(StringView path) noexcept;

    /**
     * @brief 경로를 절대 경로로 변환합니다.
     */
    static String GetFullPath(StringView path) noexcept;
};

/**
 * @class File
 *
 * @brief 파일 생성, 복사, 삭제, 이동 및 열기를 위한 정적 메서드를 제공합니다.
 */
class File final
{
    STATIC_CLASS(File)

public:
    /**
     * @brief 지정된 파일이 존재하는지 확인합니다.
     */
    static bool Exists(StringView path) noexcept;

    /**
     * @brief 파일을 삭제합니다. 존재하지 않으면 아무 동작도 하지 않습니다.
     */
    static void Delete(StringView path) noexcept;

    /**
     * @brief 파일을 새 위치로 복사합니다.
     * @param overwrite_ true면 대상 파일이 존재할 경우 덮어씁니다.
     */
    static void Copy(StringView sourceFileName, StringView destFileName, bool overwrite_ = false) noexcept;

    /**
     * @brief 파일을 새 위치로 이동합니다.
     */
    static void Move(StringView sourceFileName, StringView destFileName) noexcept;

    // --- 읽기/쓰기 편의 기능 (C# 스타일) ---

    /**
     * @brief 텍스트 파일을 열어 모든 내용을 문자열로 읽어옵니다.
     */
    static String ReadAllText(StringView path);

    /**
     * @brief 문자열을 파일에 씁니다. 파일이 있으면 덮어쓰고, 없으면 생성합니다.
     */
    static void WriteAllText(StringView path, StringView contents);

    /**
     * @brief 텍스트를 파일 끝에 추가합니다. 파일이 없으면 생성합니다.
     */
    static void AppendAllText(StringView path, StringView contents);

    /**
     * @brief 바이너리 파일을 열어 모든 내용을 바이트 배열로 읽어옵니다.
     */
    static List<u8> ReadAllBytes(StringView path);

    /**
     * @brief 바이트 배열을 파일에 씁니다.
     */
    static void WriteAllBytes(StringView path, const List<u8>& bytes);
};

/**
 * @class Directory
 *
 * @brief 디렉토리 생성, 이동 및 열거를 위한 정적 메서드를 제공합니다.
 */
class Directory final
{
    STATIC_CLASS(Directory)

public:
    /**
     * @brief 지정된 디렉토리가 존재하는지 확인합니다.
     */
    static bool Exists(StringView path) noexcept;

    /**
     * @brief 지정된 경로에 모든 디렉토리와 하위 디렉토리를 생성합니다.
     */
    static void Create(StringView path) noexcept;

    /**
     * @brief 디렉토리를 삭제합니다.
     * @param recursive_ true면 하위 디렉토리와 파일까지 모두 삭제합니다.
     */
    static void Delete(StringView path, bool recursive_ = false) noexcept;

    /**
     * @brief 디렉토리를 이동합니다.
     */
    static void Move(StringView sourceDirName, StringView destDirName) noexcept;

    /**
     * @brief 현재 디렉토리를 반환합니다.
     */
    static String GetCurrentDirectory() noexcept;

    // --- 검색 기능 ---

    /**
     * @brief 지정된 디렉토리 내의 파일 이름(경로 포함) 목록을 반환합니다.
     * * @param path_ 검색할 디렉토리 경로
     * @param searchPattern_ 검색 패턴 (예: "*.txt", 현재 C++ filesystem은 패턴 매칭을 직접 구현해야 하므로 기본은 전체
     * 검색)
     * @param recursive_ 하위 폴더까지 검색할지 여부
     */
    static List<String> GetFiles(StringView path_, StringView searchPattern_ = "*", bool recursive_ = false);

    /**
     * @brief 지정된 디렉토리 내의 하위 디렉토리 이름(경로 포함) 목록을 반환합니다.
     */
    static List<String> GetDirectories(StringView path_, bool recursive_ = false);
};