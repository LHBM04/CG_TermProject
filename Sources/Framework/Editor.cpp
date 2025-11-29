#include "Editor.h"

#include <imgui.h>

void Editor::Initialize() noexcept
{

}

void Editor::Update() noexcept
{
}

void Editor::Render() noexcept
{
    ImGui::Begin("Map Editor");

    // 모니터링 정보
    ImGui::Text("Hovered Grid: [%d, %d]", 10, 10);
    ImGui::Text("Total Objects: %d", 30);
    ImGui::Separator();

    ImGui::Text("Palette");
    if (ImGui::RadioButton("Eraser", isEraseMode))
    {
        isEraseMode = true;
    }
    ImGui::SameLine();

    if (ImGui::RadioButton("Floor", !isEraserMode && currentSelectedType == TileType::Floor))
    {
        isEraserMode        = false;
        currentSelectedType = TileType::Floor;
    }

    if (ImGui::RadioButton("Wall", !isEraserMode && currentSelectedType == TileType::Wall))
    {
        isEraserMode        = false;
        currentSelectedType = TileType::Wall;
    }

    if (ImGui::RadioButton("Spawn", !isEraserMode && currentSelectedType == TileType::SpawnPoint))
    {
        isEraserMode        = false;
        currentSelectedType = TileType::SpawnPoint;
    }

    if (ImGui::RadioButton("Goal", !isEraserMode && currentSelectedType == TileType::GoalPoint))
    {
        isEraserMode        = false;
        currentSelectedType = TileType::GoalPoint;
    }

    ImGui::Separator();

    ImGui::InputText("Filename", fileNameBuffer, sizeof(fileNameBuffer));

    if (ImGui::Button("Export (Save)"))
    {
        ExportMap();
        // 간단한 알림 팝업 오픈용 플래그 설정 가능
    }
    ImGui::SameLine();
    if (ImGui::Button("Import (Load)"))
    {
        ImportMap();
    }

    ImGui::End();
}

void Editor::ImportMap() noexcept
{
    // 구현 필요
}

void Editor::ExportMap() noexcept
{
    // 구현 필요
}

bool Editor::isEraseMode = false;