#pragma once

#include "Common.h"
#include "Scene.h"

class Object;

class Component
{
    friend class Object;

public:
    /**
     * @brief 소멸자.
     */
    virtual ~Component() noexcept = default;

    /**
     * @brief 해당 컴포넌트의 오너 엔티티를 반환합니다.
     * 
     * @return Object* 해당 컴포넌트의 오너 엔티티
     */
    [[nodiscard]]
    inline Object* GetOwner() const noexcept
    {
        return owner;
    }

    /**
     * @brief 컴포넌트의 활성화 여부를 반환합니다.
     * 
     * @return bool 해당 컴포넌트의 활성화 여부
     */
    [[nodiscard]]
    inline bool IsEnabled() const noexcept
    {
        return isEnabled;
    }

    /**
     * @brief 컴포넌트의 활성화 여부를 설정합니다.
     * 
     * @param enabled_ 해당 컴포넌트의 활성화 여부
     */
    inline void SetEnabled(bool enabled_) noexcept
    {
        isEnabled = enabled_;
    }

    /**
     * @brief 해당 컴포넌트를 파괴해야 하는지 여부를 반환합니다.
     * 
     * @bool 해당 컴포넌트를 파괴해야 하는지 여부
     */
    [[nodiscard]]
    inline bool IsDestroyed() noexcept
    {
        return isDestroyed;
    }

    /**
     * @brief 해당 컴포넌트를 파괴합니다.
     */
    inline void Destroy() noexcept
    {
        isDestroyed = true;
    }

protected:
    /**
     * @brief 컴포넌트가 생성될 때 호출됩니다. (초기화)
     */
    virtual void OnAwake()
    {
    }

    /**
     * @brief 활성화된 상태에서 첫 번째 Update 실행 직전에 호출됩니다.
     */
    virtual void OnStart()
    {
    }

    /**
     * @brief 매 프레임마다 호출됩니다.
     */
    virtual void OnUpdate()
    {
    }

    /**
     * @brief 고정된 시간 간격마다 호출됩니다. (물리 연산 등에 사용)
     */
    virtual void OnFixedUpdate()
    {
    }

    /**
     * @brief 모든 Update가 끝난 후 호출됩니다. (카메라 이동 등에 사용)
     */
    virtual void OnLateUpdate()
    {
    }

    /**
     * @brief 렌더링 시 호출됩니다.
     */
    virtual void OnRender()
    {
    }

    /**
     * @brief 컴포넌트가 파괴될 때 호출됩니다.
     */
    virtual void OnDestroy()
    {
    }

private:
    /**
     * @brief 해당 컴포넌트의 오너를 반환합니다.
     */
    Object* owner;

    /**
     * @brief 해당 컴포넌트의 활성화 여부.
     */
    bool isEnabled;

    /**
     * @brief 해당 컴포넌트의 파괴 여부.
     */
    bool isDestroyed;
};

/**
 * @class Object
 * 
 * @brief 엔티티를 정의합니다.
 */
class Object
{
public:
    /**
     * @brief 생성자.
     * 
     * @param name_ 생성할 엔티티 이름
     * @param tag_  생성할 엔티티 태그
     */
    Object(StringView name_ = "New Object", 
           StringView tag_  = "Not tag") noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Object() noexcept;

    /**
     * @brief 해당 엔티티를 매 프레임마다 갱신합니다.
     */
    void Update() noexcept;

    /**
     * @brief 해당 엔티티를 고정 프레임마다 갱신합니다.
     */
    void FixedUpdate() noexcept;

    /**
     * @brief 해당 엔티티를 매 Update 이후마다 갱신합니다
     */
    void LateUpdate() noexcept;

    /**
     * @brief 해당 엔티티를 렌더링합니다.
     */
    void Render() noexcept;

    /**
     * @brief 해당 엔티티의 이름을 가져옵니다.
     * 
     * @return String 해당 엔티티의 이름
     */
    [[nodiscard]]
    inline String GetName() const noexcept
    {
        return name;
    }

    /**
     * @brief 해당 엔티티의 이름을 지정한 이름으로 변경합니다.
     * 
     * @param name_ 지정할 이름
     */
    inline void SetName(const String& name_) noexcept
    {
        name = name_;
    }

    /**
     * @brief 해당 엔티티의 태그를 가져옵니다.
     * 
     * @return String 해당 엔티티의 태그
     */
    [[nodiscard]]
    inline String GetTag() const noexcept
    {
        return tag;
    }

    /**
     * @brief 해당 엔티티의 태그를 지정한 태그로 변경합니다.
     * 
     * @param tag_ 지정할 태그
     */
    inline void SetTag(const String& tag_) noexcept
    {
        tag = tag_;
    }

    /**
     * @brief 지정한 타입의 컴포넌트가 있는지 여부를 반환합니다.
     * 
     * @tparam TComponent 지정할 타입
     * 
     * @return bool 지정한 타입의 컴포넌트가 있는지 여부
     */
    template <typename TComponent>
    bool HasComponent() const noexcept
    {
        return false;
    }

    /**
     * @brief
     * @tparam TComponent
     * @return
     */
    template <typename TComponent>
    TComponent* AddComponent() noexcept
    {
        return nullptr;
    }

    /**
     * @brief
     * @tparam TComponent
     */
    template <typename TComponent>
    void DestroyComponent() noexcept
    {
        
    }

    /**
     * @brief 해당 엔티티의 활성화 여부를 반환합니다.
     * 
     * @return bool 해당 엔티티의 활성화 여부
     */
    template <typename TComponent>
    TComponent* GetComponent() const noexcept
    {
        return nullptr;
    }

private:
    /**
     * @brief 해당 엔티티의 이름.
     */
    String name;

    /**
     * @brief 해당 엔티티의 태그.
     */
    String tag;

    /**
     * @brief 해당 엔티티의 활성화 여부.
     */
    bool isEnabled;

    /**
     * @brief 해당 엔티티의 파괴 여부.
     */
    bool isDestroyed;

    /**
     * @brief 해당 엔티티의 핸들.
     */
    List<Unique<Component>> components;
};