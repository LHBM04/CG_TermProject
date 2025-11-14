#pragma once

class Camera final
{
public:
    /**
     * @brief 투영 방식을 정의합니다.
     */
    enum class Projection
    {
        /**
         * @brief 윈근 투영.
         */
        Perspective,

        /**
         * @brief 직교 투영.
         */
        Orthographic
    };

    /**
     * @brief 생성자.
     */
    explicit Camera() noexcept;

private:
    /**
     * @brief 투영 방식.
     */
    Camera::Projection projection;

    /**
     * @brief 시야각.
     */
    float fov;

    /**
     * @brief 종횡비.
     */
    float aspectRatio;

    /**
     * @brief 카메라가 볼 수 있는 최소 거리.
     */
    int nearPlane;

    /**
     * @brief 카메라가 볼 수 있는 최대 거리.
     */
    int farPlane;
};