#include "Spline.h"
#include "../Framework/Time.h"

void Spline::LateUpdate()
{
    // 없으면 0.0 반환
    if (controlPoints.empty())
    {
        GetTransform()->SetPosition(glm::vec3(0.0f));
        return;
    }

    // 컨트롤 포인트가 3개 이하면 마지막 점 반환
    if (controlPoints.size() < 3)
    {
        GetTransform()->SetPosition(controlPoints.back());
        return;
    }

    t += speed * TimeManager::GetDeltaTime();

    glm::vec3 p01 = (1 - t) * controlPoints[0] + t * controlPoints[1];
    glm::vec3 p12 = (1 - t) * controlPoints[1] + t * controlPoints[2];
    glm::vec3 pos = (1 - t) * p01 + t * p12;

    if (t >= 1.0f)
    {
        t = 0.0f;
        controlPoints.erase(controlPoints.begin());
        controlPoints.erase(controlPoints.begin());
    }

    GetTransform()->SetPosition(pos);
}

Spline::Spline(Object* const owner)
    : Component(owner)
    , t(0.0f)
    , speed(0.5f)
{
}

Spline::~Spline() noexcept
{
}