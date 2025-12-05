#pragma once

struct GLFWwindow

namespace Rendering
{
    struct Specification final
    {
        /**
         * @brief 애플리케이션 창 포인터.
         */
        GLFWwindow* window;

        /**
         * @brief 애플리케이션 이름.
         */
        int x;

        /**
         * @brief 애플리케이션 이름.
         */
        int y;

        /**
         * @brief 애플리케이션 창 너비.
         */
        int width;

        /**
         * @brief 애플리케이션 창 높이.
         */
        int height;
    };

    class RenderContext final
    {

    };

    /**
     * @brief 렌더링 시스템을 초기화합니다.
     * 
     * @param spec_ 렌더링 시스템 사양
     */
    void Initialize();

    /**
     * @brief 렌더링 시스템을 해제합니다.
     */
    void Release();
} // namespace Rendering