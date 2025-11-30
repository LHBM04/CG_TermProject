#pragma once

#include "../PCH.h"

class Splines
{
public:
    glm::vec3 ApplySplines();

    void AddPoint(glm::vec3 p)
    {
        controlPoints.push_back(p);
    }

    void deletePoint()
    {
        for (int i{}; i < controlPoints.size(); ++i)
        {
            controlPoints.erase(controlPoints.begin());
        }
    }

    void reset()
    {
        t = 0.0f;
    }

private:

    // 사용법
    // 
    // < 3개의 제어점을 단위로 작동 >
    // 
    // 3개의 제어점을 기준으로 베지어 곡선을 따라 변환 후
    // t가 1.0f가 되면 끝 점에 이동한 것이므로
    // 앞의 두 점을 삭제 후 끝점 부터 뒤의 2개의 점으로 3차 베지어곡선을 다시 그림
    std::vector<glm::vec3> controlPoints;

    float t = 0.0f;
    float speed = 0.2f;
};