#pragma once

#include "Common.h"
#include "Objects.h"

/**
 * @struct Viewport
 *
 * @brief 렌더링 영역을 정의합니다.
 */
struct Viewport final
{
    /**
     * @brief x 좌표.
     */
    i32 x;

    /**
     * @brief y 좌표.
     */
    i32 y;

    /**
     * @brief 너비.
     */
    i32 width;

    /**
     * @brief 높이.
     */
    i32 height;
};

class Camera final : public Component
{
public:
    /**
     * @brief 렌더링을 시작하기 전 해당 카메라를 준비합니다.
     */
    void Ready() const noexcept
    {

    }
};

class Renderer : public Component
{

};
 
class SpriteRenderer : public Renderer
{

};

class MeshRenderer : public Renderer
{
};