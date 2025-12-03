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

    Transform* tr  = GetTransform();
    glm::vec3  pos = tr->GetPosition();

    // 현재 좌표를 콘솔에 출력
    Logger::Info("Player Pos: {:.2f}, {:.2f}, {:.2f}", pos.x, pos.y, pos.z);

    // 4. 공 굴리기 시각 효과 (회전)
    float speed = glm::length(velocity);
    if (speed > 1e-6f)
    {
        // 이동 방향
        glm::vec3 moveDir = glm::normalize(velocity);
        // 회전 축 (이동 방향과 바닥 법선의 외적)
        glm::vec3 axis = glm::cross(n, moveDir);

        float axisLen = glm::length(axis);
        if (axisLen > 1e-6f)
        {
            axis = axis / axisLen; // 정규화

            // 반지름 가져오기 (Scale의 평균 사용)
            glm::vec3 scale = transform->GetScale();
            float     effectiveRadius =
                    (scale.x + scale.y + scale.z) / 3.0f; // 기본 radius가 1.0이라 가정 시 scale이 반지름 역할
            effectiveRadius = glm::max(effectiveRadius, 0.1f);

            // 회전각 계산 (s = r * theta -> theta = s / r)
            float angleRad = speed / effectiveRadius;
            float angleDeg = glm::degrees(angleRad); // 프레임워크가 Degree를 쓴다면 변환

            // 현재 회전에 추가 회전 적용 (Object의 Transform Rotate 사용)
            // World 축 기준으로 회전해야 자연스러움
            transform->Rotate(axis * angleDeg, Transform::Space::World);

            // OBB도 회전 업데이트 (필요하다면)
            myOBB->rotate(angleDeg, axis);
        }
    }
}

void PlayerController::CheckCollision(OBB* targetOBB)
{
    if (!myOBB || !targetOBB)
        return;

    // 큐브 충돌 로직 이식
    if (myOBB->testOBBOBB_SAT(*targetOBB, mtv))
    {
        // MTV 클램프: 너무 큰 튕김 방지
        float len = glm::length(mtv);
        if (len > 2.0f)
        {
            mtv *= (2.0f / len);
        }

        // 1. 위치 보정 (밀어내기)
        Transform* transform = GetTransform();
        transform->SetPosition(transform->GetPosition() + mtv);
        myOBB->move(mtv); // OBB 동기화

        // 2. 속도 반사 (입사각 반사각)
        glm::vec3 normal  = glm::normalize(mtv);
        float     dotProd = glm::dot(dir, normal);

        // 벽을 뚫고 들어가는 방향일 때만 반사
        if (dotProd < 0.0f)
        {
            dir = dir - (1.0f + cor) * dotProd * normal;
        }

        // 미세하게 띄워서 끼임 방지
        const float separationBias = 1e-3f;
        glm::vec3   bias           = separationBias * normal;
        transform->SetPosition(transform->GetPosition() + bias);
        myOBB->move(bias);
    }
}