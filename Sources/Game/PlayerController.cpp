#include "PlayerController.h"
#include "../Framework/Debug.h" // 로그용
#include "../Framework/Time.h"

PlayerController::PlayerController(Object* const owner)
    : Component(owner)
{
}

PlayerController::~PlayerController() noexcept
{
}

void PlayerController::Update()
{
    if (!myOBB)
    {
        myOBB = owner->GetComponent<OBB>();
        if (!myOBB)
            return;
    }

    float dt = TimeManager::GetDeltaTime();

    // 중력 적용
    dir += gravity * gravityScale * dt;

    // 경사면 가속 적용
    glm::vec3 n = glm::normalize(groundNormal);
    glm::vec3 g = gravity * gravityScale;

    // 경사면의 접선 벡터 계산
    glm::vec3 tangent = g - (glm::dot(g, n))*n;

    // 경사면 가속 더하기
    if (slopeBoost > 0.0f)
    {
        dir += tangent * slopeBoost * dt;
    }

    // 위치 이동
    glm::vec3  velocity  = dir * speedScale * dt;
    Transform* transform = GetTransform();

    // 현재 위치에서 이동
    glm::vec3 nextPos = transform->GetPosition() + velocity;
    transform->SetPosition(nextPos);

    // OBB도 이동
    myOBB->teleport(nextPos);

    // 공 회전
    float speed = glm::length(velocity);
    if (speed > 1e-6f)
    {
        glm::vec3 moveDir = glm::normalize(velocity);
        glm::vec3 axis    = glm::cross(n, moveDir);

        float axisLen = glm::length(axis);
        if (axisLen > 1e-6f)
        {
            axis = axis / axisLen;
            float radius = transform->GetScale().x;

            // 안전장치
            if (radius < 0.001f)
                radius = 0.5f; 

            float angleRad = speed / radius;
            float angleDeg = glm::degrees(angleRad);

            transform->Rotate(axis * angleDeg, Transform::Space::World);
        }
    }
}

void PlayerController::CheckCollision(OBB* targetOBB)
{
    if (!myOBB || !targetOBB)
        return;

    if (myOBB->testOBBOBB_SAT(*targetOBB, mtv))
    {
        // 위치 보정
        Transform* transform = GetTransform();
        transform->SetPosition(transform->GetPosition() + mtv);
        myOBB->move(mtv);

        // 속도 반사
        if (glm::length(mtv) > 1e-6f)
        {
            glm::vec3 normal  = glm::normalize(mtv);
            float     dotProd = glm::dot(dir, normal);

            // 벽 쪽으로 이동 중일 때만 반사
            if (dotProd < 0.0f)
            {
                dir = dir - (1.0f + cor) * dotProd * normal;
            }
        }
    }
}