#pragma once

#include <glm/vec3.hpp>

struct Window;

/**
 * @class Renderer
 * 
 * @brief 렌더러를 정의합니다.
 */
class Renderer final
{
public:
    /**
     * @brief 생성자.
     * 
     * @param window_ 타겟이 될 윈도우
     */
    Renderer(const Window& window_) noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Renderer() noexcept;

    /**
     * @brief 화면을 비웁니다.
     */
    void Clear() const noexcept;

    /**
     * @brief 화면을 그립니다.
     */
    void Present() const noexcept;

private:
    /**
     * @brief 지울 색상.
     */
    glm::vec3 clearColor;
};