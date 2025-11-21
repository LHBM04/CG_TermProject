#pragma once

#include "PCH.h"

class World;

class Entity
{
public:
    /**
     * @brief 
     * 
     * @param handle_ 
     * @param world_ 
     */
    explicit Entity(const entt::entity& handle_, World* const world_)
        : handle(handle_)
        , world(world_)
    {
    }

    // 1. 컴포넌트 추가
    template <typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        if (HasComponent<T>())
        {
            // 이미 있는 경우에 대한 예외 처리 (로그 출력 등)
            return GetComponent<T>();
        }
        // entt::registry::emplace 호출 (완벽한 전달)
        return scene->registry.emplace<T>(handle, std::forward<Args>(args)...);
    }

    // 2. 컴포넌트 가져오기
    template <typename T>
    T& GetComponent()
    {
        // assert(!HasComponent<T>()); // 디버그 모드에서 체크 권장
        return scene->registry.get<T>(handle);
    }

    // 3. 컴포넌트 보유 여부 확인
    template <typename T>
    bool HasComponent()
    {
        return scene->registry.all_of<T>(handle);
    }

    // 4. 컴포넌트 제거
    template <typename T>
    void RemoveComponent()
    {
        scene->registry.remove<T>(handle);
    }

    // 유효성 검사
    operator bool() const
    {
        return handle != entt::null;
    }

private:
    /**
     * @brief 해당 엔티티의 네이티브 핸들.
     */
    entt::entity handle;

    /**
     * @brief 해당 엔티티가 포함된 월드.
     */
    World* world;
};