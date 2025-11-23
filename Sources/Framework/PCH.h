#pragma once

#include <array>
#include <memory>
#include <stack>
#include <string>
#include <string_view>
#include <concepts>
#include <typeindex>
#include <typeinfo>
#include <vector>
#include <stack>
#include <random>
#include <unordered_map>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <entt/entt.hpp>

#ifdef _DEBUG
	#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
	#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <entt/entt.hpp>