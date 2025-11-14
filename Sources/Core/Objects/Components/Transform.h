#pragma once

#include "PCH.h"

#include "Component.h"

/**
 * @class Transform
 * 
 * @brief 객체의 위치, 회전, 크기 정보를 정의합니다.
 */
class Transform
	: public Component
{
public:
    /**
     * @brief 생성자.
     */
    explicit Transform() noexcept = default;

    /**
     * @brief 소멸자.
     */
    virtual ~Transform() noexcept override = default;

    /**
     * @brief 객체의 위치 값을 반환합니다.
     *
     * @return constexpr Vector2 객체의 위치 값
     */
    [[nodiscard]]
    inline constexpr Vector3 GetPosition() const noexcept;

    /**
     * @brief 객체의 좌표를 지정한 값로 설정합니다.
     * 
     * @param position_ 지정할 좌표 값
     */
    inline constexpr void SetPosition(const Vector3& position_) noexcept;

    /**
     * @brief 객체의 회전 값을 반환합니다.
     * 
     * @return constexpr Vector3 객체의 회전 값
     */
    [[nodiscard]]
    inline constexpr Vector3 GetRotation() const noexcept;

    /**
     * @brief 객체의 회전 값을 지정한 값으로 설정합니다.
     * 
     * @param rotation_ 지정할 회전 값
     */
    inline constexpr void SetRotation(const Vector3& rotation_) noexcept;

    /**
     * @brief 객체의 크기 값을 반환합니다.
     * 
     * @return constexpr Vector3 객체의 크기 값
     */
    [[nodiscard]]
    inline constexpr Vector3 GetScale() const noexcept;

    /**
     * @brief 객체의 크기 값을 지정한 값으로 설정합니다.
     * 
     * @param scale_ 지정할 크기 값
     */
    inline constexpr void SetScale(const Vector3& scale_) noexcept;

    /**
     * @brief 객체의 로컬 위치 값을 반환합니다.
     * 
     * @return constexpr Vector3 해당 객체의 로컬 위치 값
     */
    [[nodiscard]]
    inline constexpr Vector3 GetLocalPosition() const noexcept;

    /**
     * @brief 객체의 로컬 위치를 지정한 값으로 설정합니다.
     * 
     * @param localPosition_ 지정할 값
     */
    inline constexpr void SetLocalPosition(const Vector3& localPosition_) noexcept;

    /**
     * @brief 객체의 로컬 회전 값을 반환합니다.
     * 
     * @return constexpr Vector3 객체의 로컬 회전 값
     */
    [[nodiscard]]
    inline constexpr Vector3 GetLocalRotation() const noexcept;

    /**
     * @brief 객체의 로컬 회전 값을 지정한 값으로 설정합니다.
     * 
     * @param localRotation_ 지정할 로컬 회전 값
     */
    inline constexpr void SetLocalRotation(const Vector3& localRotation_) noexcept;

    /**
     * @brief 객체의 로컬 크기 값을 반환합니다.
     * 
     * @return constexpr Vector3 객체의 로컬 크기 값
     */
    [[nodiscard]]
    inline constexpr Vector3 GetLocalScale() const noexcept;

    /**
     * @brief 객체의 로컬 크기 값을 지정한 값으로 설정합니다.
     * 
     * @param localScale_ 지정할 로컬 크기 값
     */
    inline constexpr void SetLocalScale(const Vector3& localScale_) noexcept;

private:
    /**
     * @brief 객체의 월드 좌표.
     */
    Vector3 worldPosition;

    /**
     * @brief 객체의 로컬 좌표.
     */
    Vector3 localPosition;

    /**
     * @brief 객체의 월드 회전 값.
     */
    Vector3 worldRotation;

    /**
     * @brief 객체의 로컬 회전 값.
     */
    Vector3 localRotation;

    /**
     * @brief 객체의 월드 크기 값.
     */
    Vector3 worldScale;

    /**
     * @brief 객체의 로컬 크기 값.
     */
    Vector3 localScale;
};

inline constexpr Vector3 Transform::GetPosition() const noexcept
{
    return worldPosition;
}

inline constexpr void Transform::SetPosition(const Vector3& position_) noexcept
{
    worldPosition = position_;
}

inline constexpr Vector3 Transform::GetRotation() const noexcept
{
    return worldRotation;
}

inline constexpr void Transform::SetRotation(const Vector3& rotation_) noexcept
{
    worldRotation = rotation_;
}

inline constexpr Vector3 Transform::GetScale() const noexcept
{
    return worldScale;
}

inline constexpr void Transform::SetScale(const Vector3& scale_) noexcept
{
    worldScale = scale_;
}

inline constexpr Vector3 Transform::GetLocalPosition() const noexcept
{
    return localPosition;
}

inline constexpr void Transform::SetLocalPosition(const Vector3& localPosition_) noexcept
{
    localPosition = localPosition_;
}

inline constexpr Vector3 Transform::GetLocalRotation() const noexcept
{
    return localRotation;
}

inline constexpr void Transform::SetLocalRotation(const Vector3& localRotation_) noexcept
{
    localRotation = localRotation_;
}

inline constexpr Vector3 Transform::GetLocalScale() const noexcept
{
    return localScale;
}

inline constexpr void Transform::SetLocalScale(const Vector3& localScale_) noexcept
{
    localScale = localScale_;
}