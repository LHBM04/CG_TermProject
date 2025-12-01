#pragma once

// Standard
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <list>
#include <map>
#include <memory>
#include <new>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// Windows API
#include <windows.h>
#include <windowsx.h>

// GLAD(OpenGL Loader)
#include <glad/glad.h>

// GLFW(Windowing)
#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// GLM(Math Library)
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

// 정적 클래스 매크로
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
        void operator delete[](void*) = delete; 