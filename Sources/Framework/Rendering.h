#pragma once

#include <glad/glad.h>

#include "Common.h"
#include "Math.h"

/**
 * @struct Viewport
 *
 * @brief 렌더링 영역을 정의합니다.
 */
struct Viewport final
{
    /**
     * @brief x 좌표.
     */
    int x;

    /**
     * @brief y 좌표.
     */
    int y;

    /**
     * @brief 너비.
     */
    int width;

    /**
     * @brief 높이.
     */
    int height;
};

class Camera final
{
public:
    /**
     * @brief 카메라의 투영 방식을 정의합니다.
     */
    enum class Projection : std::uint8_t
    {
        /**
         * @brief 원근 투영.
         */
        Perspective,

        /**
         * @brief 직교 투영.
         */
        Orthographic
    };

    /**
     * @brief 렌더링을 시작하기 전 해당 카메라를 준비합니다.
     */
    void Ready() const noexcept;

    /**
     * @brief 
     * 
     * @return 
     */
    [[nodiscard]]
    inline float GetFov() const noexcept
    {
        return fov;
    }

    /**
     * @brief 
     * 
     * @return 
     */
    [[nodiscard]]
    inline glm::fmat4x4 GetViewMatrix() const noexcept
    {
        return glm::lookAt(eye, eye + front, up);
    }

    /**
     * @brief 
     * 
     * @return 
     */
    [[nodiscard]]
    inline glm::fmat4x4 GetProjectionMatrix() const noexcept
    {
        const float height      = viewport.height;
        const float width       = viewport.width;
        const float aspectRatio = width / height;

        switch (projection)
        {
            case Projection::Orthographic:
            {
                const float halfHeight = orthoSize * 0.5f;
                const float halfWidth  = halfHeight * aspectRatio;

                return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
            }
            case Projection::Perspective:
            {
                return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
            }
            default:
            {
                return glm::fmat4x4(1.0f);
            }
        }
    }

private:
    /**
     * @brief 해당 카메라의 투영 방식.
     */
    Camera::Projection projection;

    /**
     * @brief 해당 카메라의 뷰포트 영역.
     */
    Viewport viewport;

    /**
     * @brief 해당 카메라의 위치 벡터.
     */
    glm::fvec3 eye;

    /**
     * @brief 해당 카메라의 주시 벡터.
     */
    glm::fvec3 front;

    /**
     * @brief 해당 카메라의 윗 벡터.
     */
    glm::fvec3 up;

    /**
     * @brief 시야각.
     */
    float fov = 45.0f;

    /**
     * @brief 해당 카메라가 볼 수 있는 최소 거리.
     */
    float nearPlane = 0.1f;

    /**
     * @brief 해당 카메라가 볼 수 있는 최대 거리.
     */
    float farPlane = 100.0f;

    /**
     * @brief
     */
    float orthoSize = 10.0f;
};