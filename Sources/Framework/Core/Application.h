#ifndef GUARD_APPLICATION_H
#define GUARD_APPLICATION_H

#include "../../PCH.h"

#include "Graphics.h"
#include "Window.h"

/**
 * @class Application
 *
 * @brief 애플리케이션을 정의합니다.
 */
class Application final
{
public:
    /**
     * @struct Specification
     *
     * @brief 애플리케이션의 사양을 정의합니다.
     */
    struct Specification final
    {
        /**
         * @brief 애플리케이션의 이름.
         */
        const char* name;

        /**
         * @brief 애플리케이션의 너비.
         */
        int width;

        /**
         * @brief 애플리케이션의 높이.
         */
        int height;
    };

    /**
     * @brief 애플리케이션을 초기화합니다.
     *
     * @param specification_ 애플리케이션의 사양.
     *
     * @return bool 초기화에 성공했다면 true, 아니면 false.
     */
    static bool Initialize(const Application::Specification& specification_);

    /**
     * @brief 애플리케이션을 실행합니다.
     */
    static void Run();

private:
    /**
     * @brief 해당 애플리케이션의 사양.
     */
    static Application::Specification specification;

    /**
     * @brief 해당 애플리케이션의 윈도우.
     */
    static std::unique_ptr<Window> window;

    /**
     * @brief 해당 애플리케이션의 그래픽스.
     */
    static std::unique_ptr<Graphics> graphics;
};

#endif // !GUARD_APPLICATION_H