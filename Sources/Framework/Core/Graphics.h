#ifndef GUARD_GRAPHICS_H
#define GUARD_GRAPHICS_H

/**
 * @class Graphics
 *
 * @brief 애플리케이션 내 그래픽스를 정의합니다.
 */
class Graphics
{
public:
    /**
     * @struct Specification
     *
     * @brief 그래픽스의 사양을 정의합니다.
     */
    struct Specification final
    {
        /**
         * @brief 뷰포트의 x 좌표.
         */
        int x;

        /**
         * @brief 뷰포트의 y 좌표.
         */
        int y;

        /**
         * @brief 뷰포트의 너비.
         */
        int width;

        /**
         * @brief 뷰포트의 높이.
         */
        int height;
    };

    /**
     * @brief 생성자.
     *
     * @param specification_ 그래픽스의 사양.
     */
    explicit Graphics(const Graphics::Specification& specification_) noexcept;

    /**
     * @brief 소멸자.
     */
    ~Graphics();

    void PreRender();
    void PostRender();

private:
    /**
     * @brief 해당 그래픽스의 사양.
     */
    Graphics::Specification specification;
};

#endif // !GUARD_GRAPHICS_H