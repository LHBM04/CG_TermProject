#ifndef GUARD_APPLICATION_H
#define GUARD_APPLICATION_H

#include "../../PCH.h"

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

        /**/
        bool vSync;
    };

    /**
     * @brief 애플리케이션을 실행합니다.
     * 
     * @param specification_ 애플리케이션의 사양.
     * 
     * @return int 애플리케이션의 종료 코드.
     */
    static int Run(const Application::Specification& specification_);

    /**
     * @brief 애플리케이션을 종료합니다.
     */
    static void Quit();

private:
    Application()  = delete;
    ~Application() = delete;

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&)      = delete;

    void* operator new(std::size_t)   = delete;
    void* operator new[](std::size_t) = delete;
    void  operator delete(void*)      = delete;
    void  operator delete[](void*)    = delete;

    /**
     * @brief 윈도우를 초기화합니다.
     * 
     * @return bool 윈도우 초기화에 성공했다면 true, 그렇지 않다면 false.
     */
    static bool InitWindow() noexcept;

    /**
     * @brief 그래픽스를 초기화합니다.
     * 
     * @return bool 그래픽스 초기화에 성공했다면 true, 그렇지 않다면 false.
     */
    static bool InitGraphics() noexcept;

    /**
     * @brief 해당 애플리케이션의 사양.
     */
    static Application::Specification specification;

    /**
     * @brief 해당 애플리케이션의 윈도우.
     */
    static GLFWwindow* window;
};

#endif // !GUARD_APPLICATION_H