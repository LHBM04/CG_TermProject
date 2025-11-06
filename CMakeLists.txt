cmake_minimum_required(VERSION 3.26)
project(CG_TermProject LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(assimp        CONFIG REQUIRED)
find_package(glad          CONFIG REQUIRED)
find_package(glfw3         CONFIG REQUIRED)
find_package(glm           CONFIG REQUIRED)
find_package(imgui         CONFIG REQUIRED)
find_package(imguizmo      CONFIG REQUIRED)
find_package(nameof        CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenAL        CONFIG REQUIRED)
find_package(spdlog        CONFIG REQUIRED)

add_executable(CG_TermProject
        Sources/Framework/Core/Application.cpp
        Sources/Framework/Core/Application.h
        Sources/Framework/Core/Input.cpp
        Sources/Framework/Core/Input.h
        Sources/Framework/Core/Time.cpp
        Sources/Framework/Core/Time.h

        Sources/Game/Main.cpp
 "Sources/Framework/Core/Input.h" )

target_precompile_headers(CG_TermProject PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Sources/PCH.h")
target_include_directories(CG_TermProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(CG_TermProject PRIVATE assimp::assimp)
target_link_libraries(CG_TermProject PRIVATE glad::glad)
target_link_libraries(CG_TermProject PRIVATE glfw)
target_link_libraries(CG_TermProject PRIVATE glm::glm)
target_link_libraries(CG_TermProject PRIVATE imgui::imgui)
target_link_libraries(CG_TermProject PRIVATE imguizmo::imguizmo)
target_link_libraries(CG_TermProject PRIVATE nameof::nameof)
target_link_libraries(CG_TermProject PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(CG_TermProject PRIVATE OpenAL::OpenAL)
target_link_libraries(CG_TermProject PRIVATE spdlog::spdlog)