#include "IO.h"
#include "Debug.h"

std::string Path::Combine(const std::vector<std::string>& paths) noexcept
{
    std::filesystem::path result;
    for (const auto& path : paths)
    {
        result /= path;
    }
    return result.string();
}

std::string Path::Combine(std::string_view first_, std::string_view second_) noexcept
{
    std::filesystem::path p1(first_);
    std::filesystem::path p2(second_);
    return (p1 / p2).string();
}

std::string Path::GetFileName(std::string_view path) noexcept
{
    return std::filesystem::path(path).filename().string();
}

std::string Path::GetFileNameWithoutExtension(std::string_view path) noexcept
{
    return std::filesystem::path(path).stem().string();
}

std::string Path::GetExtension(std::string_view path) noexcept
{
    return std::filesystem::path(path).extension().string();
}

std::string Path::GetDirectoryName(std::string_view path) noexcept
{
    return std::filesystem::path(path).parent_path().string();
}

std::string Path::GetFullPath(std::string_view path) noexcept
{
    std::error_code ec;
    std::filesystem::path absolutePath = std::filesystem::absolute(path, ec);
    if (ec)
    {
        Logger::Error("Failed to get full path for '{}': {}", path, ec.message());
        return "";
    }
    return absolutePath.string();
}

bool File::Exists(std::string_view path) noexcept
{
    std::error_code ec;
    return std::filesystem::exists(path, ec) && std::filesystem::is_regular_file(path, ec);
}

void File::Delete(std::string_view path) noexcept
{
    std::error_code ec;
    if (!std::filesystem::remove(path, ec))
    {
        if (ec)
            Logger::Error("Failed to delete file '{}': {}", path, ec.message());
    }
}

void File::Copy(std::string_view sourceFileName, std::string_view destFileName, bool overwrite_) noexcept
{
    std::error_code  ec;
    std::filesystem::copy_options options = std::filesystem::copy_options::none;
    if (overwrite_)
    {
        options = std::filesystem::copy_options::overwrite_existing;
    }

    std::filesystem::copy_file(sourceFileName, destFileName, options, ec);
    if (ec)
    {
        Logger::Error("Failed to copy file from '{}' to '{}': {}", sourceFileName, destFileName, ec.message());
    }
}

void File::Move(std::string_view sourceFileName, std::string_view destFileName) noexcept
{
    std::error_code ec;
    std::filesystem::rename(sourceFileName, destFileName, ec);
    if (ec)
    {
        Logger::Error("Failed to move file from '{}' to '{}': {}", sourceFileName, destFileName, ec.message());
    }
}

std::string File::ReadAllText(std::string_view path)
{
    std::ifstream file(path.data());
    if (!file.is_open())
    {
        Logger::Error("Failed to open file for reading: {}", path);
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void File::WriteAllText(std::string_view path, std::string_view contents)
{
    std::ofstream file(path.data(), std::ios::out | std::ios::trunc);
    if (!file.is_open())
    {
        Logger::Error("Failed to open file for writing: {}", path);
        return;
    }
    file << contents;
}

void File::AppendAllText(std::string_view path, std::string_view contents)
{
    std::ofstream file(path.data(), std::ios::out | std::ios::app);
    if (!file.is_open())
    {
        Logger::Error("Failed to open file for appending: {}", path);
        return;
    }
    file << contents;
}

std::vector<unsigned char> File::ReadAllBytes(std::string_view path)
{
    // 바이너리 모드로 열고 끝(ate)으로 이동하여 파일 크기를 바로 알아냅니다.
    std::ifstream file(path.data(), std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        Logger::Error("Failed to open file for binary reading: {}", path);
        return {};
    }

    const std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg); // 다시 처음으로 이동

    if (size <= 0)
        return {};

    std::vector<unsigned char> buffer(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
    {
        Logger::Error("Failed to read binary data from: {}", path);
        return {};
    }

    return buffer;
}

void File::WriteAllBytes(std::string_view path, const std::vector<unsigned char>& bytes)
{
    std::ofstream file(path.data(), std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file.is_open())
    {
        Logger::Error("Failed to open file for binary writing: {}", path);
        return;
    }

    file.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
}

// -----------------------------------------------------------------------------
// Directory Implementation
// -----------------------------------------------------------------------------

bool Directory::Exists(std::string_view path) noexcept
{
    std::error_code ec;
    return std::filesystem::exists(path, ec) && std::filesystem::is_directory(path, ec);
}

void Directory::Create(std::string_view path) noexcept
{
    std::error_code ec;
    if (!std::filesystem::create_directories(path, ec))
    {
        // 이미 존재하는 경우는 에러가 아님
        if (ec)
            Logger::Error("Failed to create directory '{}': {}", path, ec.message());
    }
}

void Directory::Delete(std::string_view path, bool recursive_) noexcept
{
    std::error_code ec;
    if (recursive_)
    {
        std::filesystem::remove_all(path, ec);
    }
    else
    {
        std::filesystem::remove(path, ec);
    }

    if (ec)
    {
        Logger::Error("Failed to delete directory '{}': {}", path, ec.message());
    }
}

void Directory::Move(std::string_view sourceDirName, std::string_view destDirName) noexcept
{
    std::error_code ec;
    std::filesystem::rename(sourceDirName, destDirName, ec);
    if (ec)
    {
        Logger::Error("Failed to move directory from '{}' to '{}': {}", sourceDirName, destDirName, ec.message());
    }
}

std::string Directory::GetCurrentDirectory() noexcept
{
    std::error_code       ec;
    std::filesystem::path current = std::filesystem::current_path(ec);

    if (ec)
    {
        Logger::Error("Failed to get current directory: {}", ec.message());
        return "";
    }
    return current.string();
}

static bool IsMatch(std::string_view filename, std::string_view pattern)
{
    if (pattern == "*" || pattern == "*.*")
    {
        return true;
    }

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

std::vector<std::string> Directory::GetFiles(std::string_view path_, std::string_view searchPattern_, bool recursive_)
{
    std::vector<std::string> files;
    std::error_code          ec;

    if (!std::filesystem::exists(path_, ec) || !std::filesystem::is_directory(path_, ec))
    {
        Logger::Warn("Directory not found: {}", path_);
        return files;
    }

    auto iterator_logic = [&](const auto& entry)
    {
        if (entry.is_regular_file())
        {
            std::string filename = entry.path().filename().string();
            if (IsMatch(filename, searchPattern_))
            {
                files.push_back(entry.path().string());
            }
        }
    };

    if (recursive_)
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(
                     path_, std::filesystem::directory_options::skip_permission_denied, ec))
        {
            iterator_logic(entry);
        }
    }
    else
    {
        for (const auto& entry :
             std::filesystem::directory_iterator(path_, std::filesystem::directory_options::skip_permission_denied, ec))
        {
            iterator_logic(entry);
        }
    }

    return files;
}

std::vector<std::string> Directory::GetDirectories(std::string_view path_, bool recursive_)
{
    std::vector<std::string> directories;
    std::error_code          ec;

    if (!std::filesystem::exists(path_, ec) || !std::filesystem::is_directory(path_, ec))
    {
        Logger::Warn("Directory not found: {}", path_);
        return directories;
    }

    auto iterator_logic = [&](const auto& entry)
    {
        if (entry.is_directory())
        {
            directories.push_back(entry.path().string());
        }
    };

    if (recursive_)
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(
                     path_, std::filesystem::directory_options::skip_permission_denied, ec))
        {
            iterator_logic(entry);
        }
    }
    else
    {
        for (const auto& entry :
             std::filesystem::directory_iterator(path_, std::filesystem::directory_options::skip_permission_denied, ec))
        {
            iterator_logic(entry);
        }
    }

    return directories;
}