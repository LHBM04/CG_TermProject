#include "Editor.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void Editor::Initialize() noexcept
{
	// 에디터 초기화 코드 작성
}

void Editor::RenderGUI() noexcept
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // TODO
    // 여기서 ImGui 윈도우 및 위젯을 생성합니다.
    ImGui::Begin("Edit Scene");
    ImGui::Text("Welcome to the Edit Scene!");
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Quit() noexcept
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}