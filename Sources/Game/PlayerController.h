#pragma once

#include "../Framework/Objects.h"

class PlayerController : public Component
{
public:
    explicit PlayerController(Object* const owner);
    virtual ~PlayerController() noexcept;

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

    virtual void Update() override;

private:
    // 이동하려는 방향. 속도 포함
    glm::vec3 dir = glm::vec3(0.0f, 0.0f, 0.0f);

    // 충돌 시에 둘을 분리시키는 최소 이동 벡터 저장용
    glm::vec3 mtv; 

    // 중력
    const glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);

    // 큐브의 반발 계수
    const float cor = 0.3f;

    // 큐브 속도 조절용
    float gravityScale = 1.0f; // 중력 배율
    float slopeBoost   = 0.0f; // 경사면 가속 배율
    float speedScale   = 1.0f; // 최종 속도 배율
};
