#include "Camera.h"

Camera::Camera() noexcept
    : projection(Camera::Projection::Perspective)
    , fovy(45.0f)
    , aspectRatio(1.7777f)
    , nearPlane(0.1f)
    , farPlane(100.0f) 
    , pos       (glm::vec3(0.0f, 10.0f, 10.0f))
    , direction (glm::vec3(0.0f, 0.0f, 0.0f))
    , up        (glm::vec3(0.0f, 1.0f, 0.0f))
{
}

void Camera::settingCamera(GLuint shaderProgram)
{
    glm::mat4 view = glm::mat4(1.0f);
    view           = glm::lookAt(pos, direction, up);

    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projMat = glm::mat4(1.0f);

    if (projection == Projection::Perspective)
    {
        projMat = glm::perspective(fovy, aspectRatio, nearPlane, farPlane);
    }
    else if (projection == Projection::Orthographic)
    {
        projMat = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -100.0f, 100.0f);
    }

    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMat));

    unsigned int viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos");
    glUniform3f(viewPosLocation, pos.x, pos.y, pos.z);
}

void Camera::move(glm::vec3 v)
{
    pos += v;
    direction += v;
}

void Camera::moveForward()
{
    glm::vec3 dir = glm::normalize(direction - pos) * speed;
    pos += dir;
    direction += dir;
}
void Camera::moveBackward()
{
    glm::vec3 dir = glm::normalize(direction - pos) * speed;
    pos -= dir;
    direction -= dir;
}
void Camera::moveLeft()
{
    glm::vec3 dir = glm::normalize(glm::cross(direction - pos, up)) * speed;
    pos -= dir;
    direction -= dir;
}
void Camera::moveRight()
{
    glm::vec3 dir = glm::normalize(glm::cross(direction - pos, up)) * speed;
    pos += dir;
    direction += dir;
}

void Camera::moveUp()
{
    glm::vec3 dir = up * speed;
    pos += dir;
    direction += dir;
}

void Camera::moveDown()
{
    glm::vec3 dir = up * speed;
    pos -= dir;
    direction -= dir;
}

void Camera::rotate(float angle, glm::vec3 axis)
{
    glm::vec3 look     = direction - pos;
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    look               = glm::vec3(rotation * glm::vec4(look, 1.0f));
    direction          = pos + look;
}
