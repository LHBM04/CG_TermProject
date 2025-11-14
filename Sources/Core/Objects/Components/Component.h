#pragma once

class GameObject;
class Transform;

/**
 * @brief 컴포넌트를 정의합니다.
 */
class Component
{
public:
    /**
     * @brief 소멸자.
     */
    virtual ~Component() noexcept = default;

    /**
     * @brief 해당 컴포넌트의 주인을 반환합니다.
     * 
     * @return GameObject* 해당 컴포넌트의 주인
     */
    [[nodiscard]]
    inline GameObject* const GetOwner() const noexcept;

    /**
     * @brief 해당 컴포넌트의 변환 정보를 반환합니다.
     * 
     * @return Transform* const 해당 컴포넌트의 변환 정보
     */
    [[nodiscard]]
    inline Transform* const GetTransform() const noexcept;

private:
    /**
     * @brief 해당 컴포넌트의 주인.
     */
    GameObject* owner;

    /**
     * @brief 해당 컴포넌트의 변환 정보.
     */
    Transform* transform;
};

inline GameObject* const Component::GetOwner() const noexcept
{
    return owner;
}

inline Transform* const Component::GetTransform() const noexcept
{
    return transform;
}