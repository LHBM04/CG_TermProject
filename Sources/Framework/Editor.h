#pragma once

#include "Common.h"
#include "Math.h"

enum class TileType : i32
{
    None = 0,
    Floor,
    Wall,
    SpawnPoint,
    GoalPoint
};

class Editor final
{
    STATIC_CLASS(Editor)

public:
    /**
     * @brief
     */
    static void Initialize() noexcept;

    /**
     * @brief
     */
    static void Update() noexcept;

    /**
     * @brief
     */
    static void Render() noexcept;

private:
    /**
     * @brief 맵 데이터를 불러옵니다.
     */
    static void ImportMap() noexcept;

    /**
     * @brief 맵 데이터를 내보냅니다.
     */
    static void ExportMap() noexcept;

    static inline Map<std::pair<i32, i32>, TileType> mapData;

    // 에디터 상태
    static inline TileType currentSelectedType = TileType::Floor;
    static inline bool     isEraserMode        = false;
    static inline IVector2 currentHoveredGrid  = {0, 0};

    // 파일명 버퍼 (UI 입력용)
    static inline char fileNameBuffer[128] = "NewMap.json";

    /**
     * @brief 현재 지우기 모드인지 여부.
     */
    static bool isEraseMode;
};