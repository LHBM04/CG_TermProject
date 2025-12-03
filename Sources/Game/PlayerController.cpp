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
    // 내 OBB 컴포넌트 가져오기 (없으면 리턴)
    if (!myOBB)
    {
        myOBB = owner->GetComponent<OBB>();
        if (!myOBB)
            return;
    }

    float dt = TimeManager::GetDeltaTime();

    // 1. 중력 적용
    dir += gravity * gravityScale * dt;

    // 2. 경사면 가속 적용 (Cube::UpdateWithSlope 로직)
    glm::vec3 n = glm::normalize(groundNormal);
    glm::vec3 g = gravity * gravityScale;

    // 경사면의 접선 벡터 계산 (기울어진 방향)
    glm::vec3 tangent = g - (glm::dot(g, n))*n;

    // 경사면 가속 더하기
    if (slopeBoost > 0.0f)
    {
        dir += tangent * slopeBoost * dt;
    }

    // 3. 위치 이동
    glm::vec3  velocity  = dir * speedScale * dt;
    Transform* transform = GetTransform();

    // 현재 위치에서 이동
    glm::vec3 nextPos = transform->GetPosition() + velocity;
    transform->SetPosition(nextPos);

    // OBB도 이동 (OBB 구현에 따라 Transform과 별개일 수 있으므로 동기화)
    myOBB->teleport(nextPos);

    // 4. 공 굴리기 시각 효과 (회전)
    float speed = glm::length(velocity);
    if (speed > 1e-6f)
    {
        glm::vec3 moveDir = glm::normalize(velocity);
        glm::vec3 axis    = glm::cross(n, moveDir); // 회전축

        float axisLen = glm::length(axis);
        if (axisLen > 1e-6f)
        {
            axis = axis / axisLen;
            // 공의 반지름(Scale)에 따라 회전각 계산
            float radius = transform->GetScale().x;
            if (radius < 0.001f)
                radius = 0.5f; // 안전장치

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
        // 1. 뚫고 들어간 만큼 밀어내기 (위치 보정)
        Transform* transform = GetTransform();
        transform->SetPosition(transform->GetPosition() + mtv);
        myOBB->move(mtv);

        // 2. 속도 반사 (튕기기)
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