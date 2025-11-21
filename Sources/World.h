#pragma once

#include "PCH.h"

class Entity;

class World
{
    friend class Entity;

public:
    /**
     * @brief 월드를 업데이트 합니다.
     */
    void Update() noexcept;

    /**
     * @brief 월드를 그립니다.
     */
    void Render() noexcept;

private:
    /**
     * @brief 해당 월드에 포함된 엔티티.
     */
    entt::registry registry;
};