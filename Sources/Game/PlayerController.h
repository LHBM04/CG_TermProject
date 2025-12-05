#pragma once

#include "../Framework/Objects.h"
#include "../Framework/Audio.h" // 오디오 관련 헤더 포함
#include "../Framework/Resources.h" // 리소스 매니저 헤더 포함
#include "OBB.h" // OBB 헤더 포함

class PlayerController : public Component
{
public:
    explicit PlayerController(Object* const owner);
    virtual ~PlayerController() noexcept;

    inline const glm::vec3 GetDir() noexcept
    {
        return dir;
    }

    inline void SetGravityScale(const float gravityScale_) noexcept
    {
        gravityScale = gravityScale_;
    }
    inline void SetSlopeBoost(const float slopeBoost_) noexcept
    {
        slopeBoost = slopeBoost_;
    }
    inline void SetSpeedScale(const float speedScale_) noexcept
    {
        speedScale = speedScale_;
    }

    // 외부(Scene)에서 현재 바닥의 기울기를 넣어주는 함수
    inline void SetGroundNormal(const glm::vec3& normal) noexcept
    {
        groundNormal = normal;
    }

    inline void setDir(const glm::vec3 v) noexcept
    {
        dir = v;
    }

    // 충돌 처리 함수
    void CheckCollision(OBB* targetOBB);

    virtual void Awake() override
    {
        audioSource = owner->AddComponent<AudioSource>();
        wallSound = ResourceManager::LoadResource<AudioClip>("Assets\\Audio\\ballSliding.wav");
        rollingSound = ResourceManager::LoadResource<AudioClip>("Assets\\Audio\\hitWall.wav");
    }

    virtual void Update() override;

private:
    // 이동하려는 방향 (속도 포함)
    glm::vec3 dir = glm::vec3(0.0f, 0.0f, 0.0f);

    // 충돌 해결용 벡터
    glm::vec3 mtv = glm::vec3(0.0f);

    // 중력
    const glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);

    // 반발 계수
    const float cor = 0.3f;

    // 물리 파라미터
    float gravityScale = 1.0f;
    float slopeBoost   = 50.0f; // 경사면 가속도 (적절히 조절 필요)
    float speedScale   = 1.0f;

    // 바닥 기울기 (기본값: 위쪽)
    glm::vec3 groundNormal = glm::vec3(0.0f, 1.0f, 0.0f);

    // 내 OBB 컴포넌트 캐싱
    OBB* myOBB = nullptr;

    // 앙 오디오띠
    AudioSource* audioSource = nullptr;

    AudioClip* wallSound = nullptr;
    AudioClip* rollingSound = nullptr;
};