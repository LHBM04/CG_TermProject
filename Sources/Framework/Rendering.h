#pragma once

#include "Common.h"
#include "Objects.h"
#include "Resources.h"

class Camera : public Component
{
public:
    /**
     * @brief 카메라의 투영 방식을 정의합니다.
     */
    enum class Projection : std::uint8_t
    {
        /**
         * @brief 원근 투영.
         */
        Perspective,

        /**
         * @brief 직교 투영.
         */
        Orthographic
    };

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
        float x;

        /**
         * @brief y 좌표.
         */
        float y;

        /**
         * @brief 너비.
         */
        float width;

        /**
         * @brief 높이.
         */
        float height;
    };

    /**
     * @struct ClipingPlanes
     * 
     * @brief 해당 카메라의 가시 범위를 정의합니다.
     */
    struct ClipingPlanes final
    {
        /**
         * @brief 해당 카메라의 근평면 거리.
         */
        float nearPlane;

        /**
         * @brief 해당 카메라의 원평면 거리.
         */
        float farPlane;
    };

    /**
     * @brief 생성자.
     * 
     * @param owner_ 해당 컴포넌트의 오너 엔티티
     */
    explicit Camera(Object* const owner_) noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Camera() noexcept override;

    /**
     * @brief 렌더링을 시작하기 전 해당 카메라를 준비합니다.
     */
    void Ready() const noexcept;

    /**
     * @brief 해당 카메라의 투영 방식을 반환합니다.
     *
     * @return Projection 해당 카메라의 투영 방식
     */
    [[nodiscard]]
    inline Projection GetProjection() const noexcept
    {
        return projection;
    }

    /**
     * @brief 해당 카메라의 투영 방식을 설정합니다.
     *
     * @param projection_ 설정할 투영 방식
     */
    inline void SetProjection(Projection projection_) noexcept
    {
        projection = projection_;
    }

    /**
     * @brief 해당 카메라의 시야각을 반환합니다.
     *
     * @return float 해당 카메라의 시야각
     */
    [[nodiscard]]
    inline float GetFieldOfView() const noexcept
    {
        return fieldOfView;
    }

    /**
     * @brief 시야각을 설정합니다.
     *
     * @param fov_ 설정할 시야각
     */
    inline void SetFieldOfView(const float fov_) noexcept
    {
        fieldOfView = fov_;
    }

    /**
     * @brief 해당 카메라의 근평면 거리를 반환합니다.
     * 
     * @return float 해당 카메라의 근평면 거리
     */
    [[nodiscard]]
    inline const Viewport& GetViewport() const noexcept
    {
        return viewport;
    }

    /**
     * @brief 해당 카메라의 뷰포트 영역을 설정합니다.
     * 
     * @param viewport_ 해당 카메라의 뷰포트 영역
     */
    inline void SetViewport(const Viewport& viewport_) noexcept
    {
        viewport = viewport_;
    }

    /**
     * @brief 해당 카메라의 가시 범위를 반환합니다.
     * 
     * @return ClipingPlanes 해당 카메라의 가시 범위
     */
    [[nodiscard]]
    inline const ClipingPlanes& GetClipingPlanes() const noexcept
    {
        return clipingPlanes;
    }

    /**
     * @brief 해당 카메라의 가시 범위를 설정합니다.
     * 
     * @param clipPlanes_ 해당 카메라의 가시 범위
     */
    inline void SetClipingPlanes(const ClipingPlanes& clipingPlanes_) noexcept
    {
        clipingPlanes = clipingPlanes_;
    }

    /**
     * @brief 해당 카메라의 근평면 거리를 반환합니다.
     * 
     * @return float 해당 카메라의 근평면 거리
     */
    [[nodiscard]]
    inline float GetNearPlane() const noexcept
    {
        return clipingPlanes.nearPlane;
    }

    /**
     * @brief 해당 카메라의 근평면 거리를 설정합니다.
     * 
     * @param nearPlane_ 해당 카메라의 근평면 거리
     */
    inline void SetNearPlane(const float nearPlane_) noexcept
    {
        clipingPlanes.nearPlane = nearPlane_;
    }

    /**
     * @brief 해당 카메라의 원평면 거리를 반환합니다.
     * 
     * @return float 해당 카메라의 원평면 거리
     */
    [[nodiscard]]
    inline float GetFarPlane() const noexcept
    {
        return clipingPlanes.farPlane;
    }

    /**
     * @brief 해당 카메라의 원평면 거리를 설정합니다.
     * 
     * @param farPlane_ 해당 카메라의 원평면 거리
     */
    inline void SetFarPlane(const float farPlane_) noexcept
    {
        clipingPlanes.farPlane = farPlane_;
    }

    /**
     * @brief 해당 카메라의 직교 투영 크기를 반환합니다.
     * 
     * @return float 해당 카메라의 직교 투영 크기
     */
    [[nodiscard]]
    inline float GetOrthoSize() const noexcept
    {
        return orthoSize;
    }

    /**
     * @brief 해당 카메라의 직교 투영 크기를 설정합니다.
     * 
     * @param orthoSize_ 해당 카메라의 직교 투영 크기
     */
    inline void SetOrthoSize(const float orthoSize_) noexcept
    {
        orthoSize = orthoSize_;
    }

    /**
     * @brief 해당 카메라의 뷰 행렬을 반환합니다.
     * 
     * @return glm::fmat4x4 해당 카메라의 뷰 행렬
     */
    [[nodiscard]]
    inline glm::fmat4x4 GetViewMatrix() const noexcept
    {
        Transform* const transform = GetOwner()->GetTransform();
        if (!transform)
        {
            return glm::mat4(1.0f);
        }

        const glm::vec3 pos   = transform->GetPosition();
        const glm::vec3 front = transform->GetForward();
        const glm::vec3 up    = transform->GetUp();

        return glm::lookAt(pos, pos + front, up);
    }

    /**
     * @brief 해당 카메라의 투영 행렬을 반환합니다.
     * 
     * @return glm::fmat4x4 해당 카메라의 투영 행렬
     */
    [[nodiscard]]
    inline glm::fmat4x4 GetProjectionMatrix() const noexcept
    {
        const float aspectRatio = viewport.width / viewport.height;

        switch (projection)
        {
            case Camera::Projection::Orthographic:
            {
                const float halfHeight = orthoSize * 0.5f;
                const float halfWidth  = halfHeight * aspectRatio;

                return glm::ortho(-halfWidth,
                                  halfWidth,
                                  -halfHeight,
                                  halfHeight,
                                  clipingPlanes.nearPlane,
                                  clipingPlanes.farPlane);
            }
            case Camera::Projection::Perspective:
            {
                return glm::perspective(
                        glm::radians(fieldOfView), aspectRatio, clipingPlanes.nearPlane, clipingPlanes.farPlane);
            }
            default:
            {
                return glm::fmat4x4(1.0f);
            }
        }
    }

private:
    /**
     * @brief 해당 카메라의 투영 방식.
     */
    Camera::Projection projection;

    /**
     * @brief 해당 카메라의 시야각.
     */
    float fieldOfView;

    /**
     * @brief 해당 카메라의 뷰포트 영역.
     */
    Camera::Viewport viewport;

    /**
     * @brief 해당 카메라의 가시 범위.
     */
    Camera::ClipingPlanes clipingPlanes;

    /**
     * @brief 해당 카메라의 직교 투영의 크기.
     */
    float orthoSize;
};

/**
 * @brief 
 */
class Light : public Component
{
public:
    /**
     * @brief 생성자.
     * 
     * @param owner_ 해당 컴포넌트의 오너
     */
    explicit Light(Object* const owner_) noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Light() noexcept override;

    /**
     * @brief 해당 조명의 색상을 반환합니다.
     * * @return glm::fvec3 조명의 색상
     */
    [[nodiscard]]
    inline const glm::fvec3& GetColor() const noexcept
    {
        return color;
    }

    /**
     * @brief 해당 조명의 색상을 설정합니다.
     * * @param color_ 설정할 색상
     */
    inline void SetColor(const glm::fvec3& color_) noexcept
    {
        color = color_;
    }

    /**
     * @brief 해당 조명의 세기를 반환합니다.
     * 
     * @return float 조명의 세기
     */
    [[nodiscard]]
    inline float GetIntensity() const noexcept
    {
        return intensity;
    }

    /**
     * @brief 해당 조명의 세기를 설정합니다.
     * 
     * @param intensity_ 조명의 세기
     */
    inline void SetIntensity(const float intensity_) noexcept
    {
        intensity = intensity_;
    }

protected:
    /**
     * @brief 매 프레임마다 호출됩니다.
     */
    virtual void Update() noexcept override;

private:
    /**
     * @brief 해당 조명의 색상.
     */
    glm::fvec3 color;

    /**
     * @brief 해당 조명의 세기.
     */
    float intensity;
};

/**
 * @class MeshRenderer
 *
 * @brief 메쉬를 렌더링하는 컴포넌트를 정의합니다.
 */
class MeshRenderer : public Component
{
public:
    /**
     * @brief 생성자.
     * 
     * @param owner_ 해당 컴포넌트의 오너
     */
    explicit MeshRenderer(Object* const owner_) noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~MeshRenderer() noexcept override;

    /**
     * @brief 해당 메쉬를 렌더링합니다.
     */
    virtual void Render() noexcept override;

    /**
     * @brief 해당 렌더러가 그릴 메쉬를 설정합니다.
     * 
     * @param mesh_ 해당 렌더러가 그릴 메쉬
     */
    [[nodiscard]]
    inline Mesh* const GetMesh() const noexcept
    {
        return mesh;
    }

    /**
     * @brief 해당 렌더러가 그릴 메쉬를 설정합니다.
     * 
     * @param mesh_ 해당 렌더러가 그릴 메쉬
     */
    inline void SetMesh(Mesh* const mesh_) noexcept
    {
        mesh = mesh_;
    }

    /**
     * @brief 해당 렌더러가 사용할 텍스처를 반환합니다.
     * 
     * @return Texture* 해당 렌더러가 사용할 텍스처
     */
    [[nodiscard]]
    inline Texture* const GetTexture() const noexcept
    {
        return texture;
    }

    /**
     * @brief 해당 렌더러가 사용할 텍스처를 설정합니다.
     * 
     * @param texture_ 해당 렌더러가 사용할 텍스처
     */
    inline void SetTexture(Texture* const texture_) noexcept
    {
        texture = texture_;
    }

private:
    /**
     * @brief 해당 렌더러가 그릴 메쉬.
     */
    Mesh* mesh;

    /**
     * @brief 해당 렌더러가 사용할 텍스처.
     */
    Texture* texture;
};