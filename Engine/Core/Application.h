#pragma once

#include <cstddef>
#include <string>

struct GLFWwindow;

/**
 * @class Application
 * 
 * @brief 애플리케이션을 정의합니다.
 */
class Application
{
public:
    /**
     * @enum WindowMode
     *
     * @brief 윈도우가 가질 수 있는 상태를 정의합니다.
     */
    enum class WindowMode : unsigned char
    {
        /**
         * @brief 창 모드.
         */
        Windowed,

        /**
         * @brief 전체 화면.
         */
        FullScreen,

        /**
         * @brief 테두리 없는 창 모드.
         */
        Borderless
    };

    /**
     * @struct Specification
     * 
     * @brief 애플리케이션의 사양을 정의합니다.
     */
    struct Specification final
    {
        /**
         * @brief 애플리케이션 이름.
         */
        std::string name;

        /**
         * @brief 애플리케이션 창 너비.
         */
        int width;

        /**
         * @brief 애플리케이션 창 높이.
         */
        int height;

        /**
         * @brief 전체 화면 모드 사용 여부.
         */
        WindowMode screenMode;

        /**
         * @brief 수직 동기화 활성화 여부.
         */
        bool sholudVSync;
    };

#pragma region Deleted Functions
    Application()  = delete;
    ~Application() = delete;

    Application(const Application&)            = delete;
    Application& operator=(const Application&) = delete;

    Application(Application&&)            = delete;
    Application& operator=(Application&&) = delete;

    void* operator new(std::size_t)     = delete;
    void* operator new[](std::size_t) = delete;
    void operator delete(void*)   = delete;
    void operator delete[](void*) = delete;

#pragma endregion

    /**
     * @brief 네이티브 윈도우 핸들을 반환합니다.
     * 
     * @return GLFWwindow* const 네이티브 윈도우 핸들.
     */
    [[nodiscard]]
    inline static GLFWwindow* const GetNativeWindow() noexcept;

    /**
     * @brief 애플리케이션을 초기화합니다.
     * 
     * @param specification_ 애플리케이션 사양.
     * 
     * @return bool 초기화 성공 여부.
     */
    static bool Initialize(const Application::Specification& specification_) noexcept;

    /**
     * @brief 애플리케이션을 실행합니다.

     * @return int 애플리케이션의 종료 코드.
     */
    static int Run() noexcept;

    /**
     * @brief 애플리케이션을 종료합니다.
     */
    static void Quit() noexcept;

private:
    /**
     * @brief 애플리케이션의 사양.
     */
    static Specification specification;

    /**
     * @brief 애플리케이션의 윈도우 핸들.
     */
    static volatile GLFWwindow* window;
};

inline GLFWwindow* const Application::GetNativeWindow() noexcept
{
    return const_cast<GLFWwindow*>(window);
}