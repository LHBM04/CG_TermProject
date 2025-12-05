#include "Path.h"

std::filesystem::path Path::Combine(std::span<const std::filesystem::path> paths_)
{
    std::filesystem::path result;
    for (const std::filesystem::path& path : paths_)
    {
        result /= path;
    }

    return result;
}

std::filesystem::path Path::Combine(const std::filesystem::path& first_, 
                                    const std::filesystem::path& second_)
{
    return first_ / second_;
}

std::filesystem::path Path::GetFileName(const std::filesystem::path& path_)
{
    return path_.filename();
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
        throw std::runtime_error("Failed to get full path for '" + path.string() + "': " + ec.message());
    }
    return absolutePath;
}