#pragma once

#include "../PCH.h"

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

    const glm::vec3 getPos() const
    {
        return pos;
    }

    void move(glm::vec3 v);
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void rotate(float angle, glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f));

    void settingCamera(GLuint shaderProgram);

private:
    /**
     * @brief 투영 방식.
     */
    Camera::Projection projection;

    glm::vec3 pos; 
    glm::vec3 direction; 
    glm::vec3 up;

    const float speed = 0.2f;

    /**
     * @brief 시야각.
     */
    float fovy;

    /**
     * @brief 종횡비.
     */
    float aspectRatio;

    /**
     * @brief 카메라가 볼 수 있는 최소 거리.
     */
    float nearPlane;

    /**
     * @brief 카메라가 볼 수 있는 최대 거리.
     */
    float farPlane;
};