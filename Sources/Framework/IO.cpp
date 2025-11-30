#include "IO.h"

#include "Debug.h"

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

std::vector<std::string> Directory::GetFiles(std::string_view path_, std::string_view searchPattern_, bool recursive_)
{
    std::vector<std::string> files;

    if (recursive_)
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path_))
        {
            if (std::filesystem::is_regular_file(entry))
            {
                files.push_back(entry.path().string());
            }
        }
    }
    else
    {
        for (const auto& entry : std::filesystem::directory_iterator(path_))
        {
            if (std::filesystem::is_regular_file(entry))
            {
                files.push_back(entry.path().string());
            }
        }
    }

    return files;
}