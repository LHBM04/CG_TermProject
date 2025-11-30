#include "Splines.h"
#include "../Core/Time.h"

glm::vec3 Splines::ApplySplines()
{
    
    // 없으면 0 반환
    if (controlPoints.empty())
    {
        return glm::vec3(0.0f);
    }
    // 컨트롤 포인트가 3개 이하면 마지막 점 반환
    if (controlPoints.size() < 3)
    {
        return controlPoints.back();
    }

    t += speed * Time::GetDeltaTime();

    glm::vec3 p01 = (1 - t) * controlPoints[0] + t * controlPoints[1];
    glm::vec3 p12 = (1 - t) * controlPoints[1] + t * controlPoints[2];
    glm::vec3 pos = (1 - t) * p01 + t * p12;

    if (t >= 1.0f)
    {
        t = 0.0f;
        controlPoints.erase(controlPoints.begin());
        controlPoints.erase(controlPoints.begin());
    }

    return pos;
}