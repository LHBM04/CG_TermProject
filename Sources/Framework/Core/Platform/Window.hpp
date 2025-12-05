#pragma once

#include <string>

struct GLFWmonitor;
struct GLFWwindow;

class Window
{
public:
    struct Specification final
    {
        /**
         * @brief OpenGL 컨텍스트 메이저 버전.
         */
        int contextVersionMajor = Window::DEFAULT_CONTEXT_VERSION_MAJOR;

        /**
         * @brief OpenGL 컨텍스트 마이너 버전.
         */
        int contextVersionMinor = Window::DEFAULT_CONTEXT_VERSION_MINOR;

        /**
         * @brief 모니터 핸들.
         */
        GLFWmonitor* monitor = nullptr;

        /**
         * @brief 창 제목.
         */
        std::string title = "New Application";

        /**
         * @brief 창 위치 X 좌표.
         */
        int x = 100;

        /**
         * @brief 창 위치 Y 좌표.
         */
        int y = 100;

        /**
         * @brief 창 너비.
         */
        int width = 800;

        /**
         * @brief 창 높이.
         */
        int height = 600;
        
        /**
         * @brief 전체 화면 여부.
         */
        bool fullscreen = false;
        
        /**
         * @brief 창 조절 가능 여부.
         */
        bool resizable = true;
        
        /**
         * @brief 창 테두리 표시 여부.
         */
        bool decorated = true;
      
        /**
         * @brief 수직 동기화 활성화 여부.
         */
        bool vSync = false;

        /**
         * @brief 화면 새로 고침 빈도.
         */
        int refreshRate = 60;
    };

    /**
     * @brief 생성자.
     * 
     * @param specification_ 생성할 창 사양.
     */
    explicit Window(const Window::Specification& specification_) noexcept;

    /**
     * @brief 소멸자.
     */
    ~Window() noexcept;

    /**
     * @brief 창 전체 화면 여부를 반환합니다.
     * 
     * @return bool 창 전체 화면 여부
     */
    [[nodiscard]]
    bool IsFullscreen() const noexcept;

    /**
     * @brief 창 전체 화면 여부를 설정합니다.
     * 
     * @param enabled_ 창 전체 화면 여부
     */
    void SetFullscreen(const bool enabled_) noexcept;

    /**
     * @brief 창 테두리 표시 여부를 반환합니다.
     *
     * @return bool 창 테두리 표시 여부
     */
    [[nodiscard]]
    bool IsDecorated() const noexcept;

    /**
     * @brief 창 테두리 표시 여부를 설정합니다.
     *
     * @param enabled_ 창 테두리 표시 여부
     */
    void SetDecorated(const bool enabled_) noexcept;

    /**
     * @brief 창 조절 가능 여부를 반환합니다.
     * 
     * @return bool 창 조절 가능 여부
     */
    [[nodiscard]]
    bool IsResizable() const noexcept;

    /**
     * @brief 창 조절 가능 여부를 설정합니다.
     * 
     * @param enabled_ 창 조절 가능 여부
     */
    void SetResizable(const bool enabled_) noexcept;

    /**
     * @brief 수직 동기화 활성화 여부를 반환합니다.
     * 
     * @return bool 수직 동기화 활성화 여부
     */
    [[nodiscard]]
    bool IsVSync() const noexcept;

    /**
     * @brief 수직 동기화 활성화 여부를 설정합니다.
     * 
     * @param enabled_ 수직 동기화 활성화 여부
     */
    void SetVSync(const bool enabled_) noexcept;

    /**
     * @brief 창 네이티브 핸들을 반환합니다.
     * 
     * @return GLFWwindow* const 창 네이티브 핸들
     */
    [[nodiscard]]
    GLFWwindow* const GetNativeHandle() const noexcept;

private:
    /**
     * @brief 기본 컨텍스트 메이저 버전.
     */
    static constexpr int DEFAULT_CONTEXT_VERSION_MAJOR = 4;

    /**
     * @brief 기본 컨텍스트 마이너 버전.
     */
    static constexpr int DEFAULT_CONTEXT_VERSION_MINOR = 5;

    /**
     * @brief 해당 창의 사양.
     */
    Window::Specification specification;

    /**
     * @brief 해당 창의 네이티브 핸들.
     */
    GLFWwindow* native;
}; // class Window