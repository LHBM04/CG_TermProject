#pragma once

struct GLFWwindow;

/**
 * @class Window
 * 
 * @brief 애플리케이션 내 윈도우를 정의합니다.
 */
class Window
{
public:
	/**
     * @struct Specification
	 *
	 * @brief 윈도우의 사양을 정의합니다.
	 */
	struct Specification final
	{
        /**
         * @brief 윈도우의 컨텍스트 상위 버전.
         */
        int contextMajorVersion;

		/**
		 * @brief 윈도우의 컨텍스트 하위 버전.
		 */
		int contextMinorVersion;

		/**
		 * @brief 윈도우의 제목.
		 */
		const char* title;

		/**
		 * @brief 윈도우의 너비.
		 */
		int width;

		/**
		 * @brief 윈도우의 높이.
		 */
		int height;

		/**
		 * @brief 윈도우의 전체 화면 모드 활성화 여부.
         */
		bool isFullScreen;

		/**
         * @brief 윈도우의 테두리 제거 여부.
		 */
		bool isBorderless;

		/**
         * @brief 윈도우의 수직 동기화 활성화 여부.
		 */
		bool isVSync;

		/**
		 * @brief 윈도우의 리사이즈 가능 여부.
         */
		bool isResizable;
	};

	/**
	 * @brief 생성자.
	 * 
	 * @param specification_ 생성할 윈도우의 사양
	 */
	Window(const Window::Specification& specification_) noexcept;

	/**
	 * @brief 소멸자.
     */
	virtual ~Window();

	/**
     * @brief 해당 윈도우의 네이티브 핸들을 반환합니다.
	 * 
	 * @return GLFWwindow* const 네이티브 윈도우 핸들.
	 */
	[[nodiscard]]
	inline GLFWwindow* const GetNativeHandle() const noexcept;

	/**
	 * @brief 해당 윈도우의 사양을 반환합니다.
	 * 
	 * @return 
	 */
	[[nodiscard]]
    inline const Window::Specification& GetSpecification() const noexcept;

private:
    /**
     * @brief 해당 윈도우의 사양.
     */
    Window::Specification specification;

	/**
	 * @brief 네이티브 윈도우 핸들.
     */
	GLFWwindow* handle;
};

inline GLFWwindow* const Window::GetNativeHandle() const noexcept
{
	return handle;
}

inline const Window::Specification& Window::GetSpecification() const noexcept
{
    return specification;
}