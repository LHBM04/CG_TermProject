#pragma once

#include "Common.h"

/**
 * @class Application
 *
 * @brief 애플리케이션을 정의합니다.
 */
class Application final
{
    STATIC_CLASS(Application)

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
        Application::WindowMode screenMode;

        /**
         * @brief 수직 동기화 활성화 여부.
         */
        bool sholudVSync;
    };

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

    /**
     * @brief 애플리케이션의 윈도우 핸들을 반환합니다.
     *
     * @return GLFWwindow* 애플리케이션의 윈도우 핸들.
     */
    [[nodiscard]]
    static inline GLFWwindow* GetNativeHandle() noexcept
    {
        return window;
    }

    /**
     * @brief 애플리케이션 이름을 반환합니다.
     * 
     * @return char* 애플리케이션 이름.
     */
    [[nodiscard]]
    static inline char* GetName() noexcept
    {
        return specification.name.data();
    }

    /**
     * @brief 애플리케이션 이름을 설정합니다.
     * 
     * @param name_ 설정할 애플리케이션 이름.
     */
    static inline void SetName(std::string_view name_) noexcept
    {
        specification.name = name_;
        glfwSetWindowTitle(window, specification.name.c_str());
    }

    /**
     * @brief 해당 애플리케이션의 창 너비를 반환합니다.
     * 
     * @return int 해당 애플리케이션의 창 너비.
     */
    [[nodiscard]]
    static inline int GetWidth() noexcept
    {
        return specification.width;
    }

    /**
     * @brief 해당 애플리케이션의 창 너비를 지정한 사이즈로 설정합니다.
     * 
     * @param width_ 지정할 사이즈
     */
    static inline void SetWidth(int width_) noexcept
    {
        specification.width = width_;
        glfwSetWindowSize(window, specification.width, specification.height);
    }

    /**
     * @brief 해당 애플리케이션의 창 높이를 반환합니다.
     * 
     * @return int 해당 애플리케이션의 창 높이.
     */
    [[nodiscard]]
    static inline int GetHeight() noexcept
    {
        return specification.height;
    }

    /**
     * @brief 해당 애플리케이션의 창 높이를 지정한 사이즈로 설정합니다.
     * 
     * @param height_ 지정할 사이즈
     */
    static inline void SetHeight(int height_) noexcept
    {
        specification.height = height_;
        glfwSetWindowSize(window, specification.width, specification.height);
    }

private:
    /**
     * @brief 애플리케이션 상태를 업데이트합니다.
     */
    static void Update() noexcept;

    /**
     * @brief 애플리케이션을 렌더링합니다.
     */
    static void Render() noexcept;

#if defined(DEBUG) || defined(_DEBUG)
    /**
     * @brief 디버그 콜백 함수.
     */
    static void OnDebugMessage(unsigned int source_,
                               unsigned int type_,
                               unsigned int id_,
                               unsigned int severity_,
                               int          length_,
                               const char*  message_,
                               const void*  userParam_) noexcept;
#endif

    /**
     * @brief 애플리케이션의 사양.
     */
    static Specification specification;

    /**
     * @brief 애플리케이션의 윈도우 핸들.
     */
    static GLFWwindow* window;

    /**
     * @brief 클리어 컬러.
     */
    static glm::vec3 clearColor;
};