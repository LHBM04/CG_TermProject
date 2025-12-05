#include "IO.h"
#include <fstream>
#include <sstream>
#include "Debug.h"
#include "Path.h"

// -----------------------------------------------------------------------------
// Path Implementation
// -----------------------------------------------------------------------------

std::filesystem::path Path::Combine(const std::vector<std::filesystem::path>& paths)
{
    std::filesystem::path result;
    for (const auto& p : paths)
    {
        result /= p;
    }
    return result;
}

std::filesystem::path Path::Combine(const std::filesystem::path& path1, const std::filesystem::path& path2)
{
    return path1 / path2;
}

std::filesystem::path Path::GetFileName(const std::filesystem::path& path)
{
    return path.filename();
}

std::string Path::GetFileNameWithoutExtension(const std::filesystem::path& path)
{
    return path.stem().string();
}

std::string Path::GetExtension(const std::filesystem::path& path)
{
    return path.extension().string();
}

std::filesystem::path Path::GetDirectoryName(const std::filesystem::path& path)
{
    return path.parent_path();
}

std::filesystem::path Path::GetFullPath(const std::filesystem::path& path)
{
    std::error_code       ec;
    std::filesystem::path absolutePath = std::filesystem::absolute(path, ec);
    if (ec)
    {
        Logger::Error("Failed to get full path for '{}': {}", path.string(), ec.message());
        return {};
    }
    return absolutePath;
}

// -----------------------------------------------------------------------------
// File Implementation
// -----------------------------------------------------------------------------

bool File::Exists(const std::filesystem::path& path)
{
    std::error_code ec;
    // exists만으로는 디렉토리일 수도 있으므로 is_regular_file 체크 필요
    return std::filesystem::exists(path, ec) && std::filesystem::is_regular_file(path, ec);
}

void File::Delete(const std::filesystem::path& path)
{
    std::error_code ec;
    if (!std::filesystem::remove(path, ec))
    {
        // 파일이 없어서 삭제 못한 건 에러가 아니라고 판단할 수도 있습니다.
        if (ec)
            Logger::Error("Failed to delete file '{}': {}", path.string(), ec.message());
    }
}

void File::Copy(const std::filesystem::path& source, const std::filesystem::path& destination, bool overwrite)
{
    std::error_code ec;
    auto            options = std::filesystem::copy_options::none;
    if (overwrite)
    {
        options = std::filesystem::copy_options::overwrite_existing;
    }

    std::filesystem::copy_file(source, destination, options, ec);
    if (ec)
    {
        Logger::Error("Failed to copy file from '{}' to '{}': {}", source.string(), destination.string(), ec.message());
    }
}

void File::Move(const std::filesystem::path& source, const std::filesystem::path& destination)
{
    std::error_code ec;
    std::filesystem::rename(source, destination, ec);
    if (ec)
    {
        Logger::Error("Failed to move file from '{}' to '{}': {}", source.string(), destination.string(), ec.message());
    }
}

std::string File::ReadAllText(const std::filesystem::path& path)
{
    // path 객체를 바로 사용 (C++17)
    std::ifstream file(path);
    if (!file.is_open())
    {
        Logger::Error("Failed to open file for reading: {}", path.string());
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void File::WriteAllText(const std::filesystem::path& path, std::string_view contents)
{
    std::ofstream file(path, std::ios::out | std::ios::trunc);
    if (!file.is_open())
    {
        Logger::Error("Failed to open file for writing: {}", path.string());
        return;
    }
    file << contents;
}

void File::AppendAllText(const std::filesystem::path& path, std::string_view contents)
{
    std::ofstream file(path, std::ios::out | std::ios::app);
    if (!file.is_open())
    {
        Logger::Error("Failed to open file for appending: {}", path.string());
        return;
    }
    file << contents;
}

std::vector<unsigned char> File::ReadAllBytes(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        Logger::Error("Failed to open file for binary reading: {}", path.string());
        return {};
    }

    const std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size <= 0)
        return {};

    std::vector<unsigned char> buffer(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
    {
        Logger::Error("Failed to read binary data from: {}", path.string());
        return {};
    }

    return buffer;
}

void File::WriteAllBytes(const std::filesystem::path& path, const std::vector<unsigned char>& bytes)
{
    std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file.is_open())
    {
        Logger::Error("Failed to open file for binary writing: {}", path.string());
        return;
    }

    file.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
}

// -----------------------------------------------------------------------------
// Directory Implementation
// -----------------------------------------------------------------------------

bool Directory::Exists(const std::filesystem::path& path)
{
    std::error_code ec;
    return std::filesystem::exists(path, ec) && std::filesystem::is_directory(path, ec);
}

void Directory::Create(const std::filesystem::path& path)
{
    std::error_code ec;
    if (!std::filesystem::create_directories(path, ec))
    {
        if (ec) // 이미 존재하는 경우 false 반환하지만 ec는 0임. 에러가 있을 때만 로깅
            Logger::Error("Failed to create directory '{}': {}", path.string(), ec.message());
    }
}

void Directory::Delete(const std::filesystem::path& path, bool recursive)
{
    std::error_code ec;
    if (recursive)
    {
        std::filesystem::remove_all(path, ec);
    }
    else
    {
        std::filesystem::remove(path, ec);
    }

    if (ec)
    {
        Logger::Error("Failed to delete directory '{}': {}", path.string(), ec.message());
    }
}

void Directory::Move(const std::filesystem::path& source, const std::filesystem::path& destination)
{
    std::error_code ec;
    std::filesystem::rename(source, destination, ec);
    if (ec)
    {
        Logger::Error(
                "Failed to move directory from '{}' to '{}': {}", source.string(), destination.string(), ec.message());
    }
}

std::filesystem::path Directory::GetCurrentDirectory()
{
    std::error_code       ec;
    std::filesystem::path current = std::filesystem::current_path(ec);

    if (ec)
    {
        Logger::Error("Failed to get current directory: {}", ec.message());
        return {};
    }
    return current;
}

static bool IsMatch(std::string_view filename, std::string_view pattern)
{
    if (pattern == "*" || pattern == "*.*")
        return true;

    if (pattern.starts_with("*."))
    {
        std::string_view ext = pattern.substr(1);
        if (filename.length() >= ext.length())
        {
            return filename.compare(filename.length() - ext.length(), ext.length(), ext) == 0;
        }
    }

    return filename == pattern;
}

std::vector<std::filesystem::path>
        Directory::GetFiles(const std::filesystem::path& path, std::string_view searchPattern, bool recursive)
{
    std::vector<std::filesystem::path> files;
    std::error_code                    ec;

    if (!std::filesystem::exists(path, ec) || !std::filesystem::is_directory(path, ec))
    {
        Logger::Warn("Directory not found: {}", path.string());
        return files;
    }

    if (recursive)
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(
                     path, std::filesystem::directory_options::skip_permission_denied, ec))
        {
            if (entry.is_regular_file())
            {
                std::string filename = entry.path().filename().string();
                if (IsMatch(filename, searchPattern))
                {
                    files.push_back(entry.path());
                }
            }
        }
    }
    else
    {
        for (const auto& entry :
             std::filesystem::directory_iterator(path, std::filesystem::directory_options::skip_permission_denied, ec))
        {
            if (entry.is_regular_file())
            {
                std::string filename = entry.path().filename().string();
                if (IsMatch(filename, searchPattern))
                {
                    files.push_back(entry.path());
                }
            }
        }
    }

    return files;
}

std::vector<std::filesystem::path> Directory::GetDirectories(const std::filesystem::path& path, bool recursive)
{
    std::vector<std::filesystem::path> directories;
    std::error_code                    ec;

    if (!std::filesystem::exists(path, ec) || !std::filesystem::is_directory(path, ec))
    {
        Logger::Warn("Directory not found: {}", path.string());
        return directories;
    }

    auto process_entry = [&](const std::filesystem::directory_entry& entry)
    {
        if (entry.is_directory())
        {
            directories.push_back(entry.path());
        }
    };

    if (recursive)
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(
                     path, std::filesystem::directory_options::skip_permission_denied, ec))
        {
            process_entry(entry);
        }
    }
    else
    {
        for (const auto& entry :
             std::filesystem::directory_iterator(path, std::filesystem::directory_options::skip_permission_denied, ec))
        {
            process_entry(entry);
        }
    }

    return directories;
}