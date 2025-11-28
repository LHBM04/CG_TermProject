#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <format>
#include <memory>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <spdlog/spdlog.h>

#define STATIC_CLASS(type)                      \
	public:                                     \
	    type() = delete;                        \
        ~type() = delete;                       \
                                                \
        type(const type&) = delete;             \
        type& operator=(const type&) = delete;  \
                                                \
        type(type&&) = delete;                  \
        type& operator=(type&&) = delete;       \
                                                \
        void* operator new(size_t) = delete;    \
        void* operator new[](size_t) = delete;  \
        void operator delete(void*) = delete;   \
        void operator delete[](void*) = delete; \

class Logger final
{
    STATIC_CLASS(Logger)

public:
    template<typename... Args>
    static void Trace(std::string_view message_, Args... args_) noexcept
    {
        Log(Level::Trace, std::vformat(message_, std::make_format_args(args_...)));
    }

    template <typename... Args>
    static void Info(std::string_view message_, Args... args_) noexcept
    {
        Log(Level::Info, std::vformat(message_, std::make_format_args(args_...)));
    }

    template <typename... Args>
    static void Warning(std::string_view message_, Args... args_) noexcept
    {
        Log(Level::Warning, std::vformat(message_, std::make_format_args(args_...)));
    }

    template <typename... Args>
    static void Error(std::string_view message_, Args... args_) noexcept
    {
        Log(Level::Error, std::vformat(message_, std::make_format_args(args_...)));
    }

    template <typename... Args>
    static void Critical(std::string_view message_, Args... args_) noexcept
    {
        Log(Level::Critical, std::vformat(message_, std::make_format_args(args_...)));
    }

private:
    enum class Level : unsigned char
    {
        Trace,
        Info,
        Warning,
        Error,
        Critical,
    };

    static void Initialize() noexcept;

    static void Log(Level level_, std::string_view message_) noexcept;

    static std::shared_ptr<spdlog::logger> s_CoreLogger
};

using Vector2Float = glm::vec2;
using Vector2Int   = glm::ivec2;

using Vector3Float = glm::vec3;
using Vector3Int   = glm::ivec3;

using Vector4Float = glm::vec4;
using Vector4Int   = glm::ivec4;

using Matrix4x4	= glm::mat4;