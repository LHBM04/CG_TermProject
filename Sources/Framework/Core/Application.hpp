#pragma once

#include <string>

namespace Application
{
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
     * @enum Specification
     *
     * @brief 애프리케이션의 사양을 정의합니다.
     */
    struct Specification final
    {
        /**
         * @brief 애플리케이션 이름.
         */
        std::string name;

        /**
         * @brief 
         */
        std::string version;

        /**
         * @brief 
         */
        std::string company;

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
         * @brief 창 모드.
         */
        Application::WindowMode windowMode;

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
     * @brief 애플리케이션을 초기화합니다.
     *
     * @param specification_ 애플리케이션 사양
     *
     * @return bool 초기화 성공 여부
     */
    bool Initialize(const Application::Specification& specification_);

    /**
     * @brief 
     * 
     * @return int 애플리케이션 종료 코드
     */
    int Run();

    /**
     * @brief 애플리케이션을 종료합니다.
     * 
     * @param quitCode_ 종료 코드
     */
    void Quit(const int exitCode_);

    /**
     * @brief 해당 애플리케이션의 실행 여부를 반환합니다.
     * 
     * @return bool 해당 애플리케이션의 실행 여부
     */
    [[nodiscard]]
    bool IsRunning();

    /**
     * @brief 애플리케이션의 이름을 반환합니다.
     * 
     * @return const std::string& 애플리케이션의 이름
     */
    [[nodiscard]]
    const std::string& GetName();

    /**
     * @brief 애플리케이션 이름을 설정합니다.
     *
     * @param name_ 설정할 애플리케이션 이름.
     */
    void SetName(std::string_view name_);

    /**
     * @brief 애플리케이션의 버전을 반환합니다.
     * 
     * @return const std::string& 애플리케이션의 버전
     */
    [[nodiscard]]
    const std::string& GetVersion();

    /**
     * @brief 애플리케이션 버전을 설정합니다.
     *
     * @param version_ 설정할 애플리케이션 버전.
     */
    void SetVersion(std::string_view version_);

    /**
     * @brief 애플리케이션의 회사를 반환합니다.
     * 
     * @return const std::string& 애플리케이션의 회사
     */
    [[nodiscard]]
    const std::string& GetCompany();

    /**
     * @brief 애플리케이션 회사를 설정합니다.
     *
     * @param company_ 설정할 애플리케이션 회사.
     */
    void SetCompany(std::string_view company_);
} // namespace Application