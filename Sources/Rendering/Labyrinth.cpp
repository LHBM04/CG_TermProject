#include "Labyrinth.h"

Labyrinth::Labyrinth()
{
    for (int i{-7}; i <= 7; ++i)
    {
        for (int j{-7}; j <= 7; ++j)
        {
            if (i == -7 || i == 7 || j == -7 || j == 7)
            {
                map.push_back(new Cube());
                map.back()->setTexture(wood_texture3);
                map.back()->move(glm::vec3(float(i), 0.5f, float(j)));
                map.back()->resize(glm::vec3(0.5f, 0.5f, 0.5f));
            }
        }
    }
    map.push_back(new Cube());
    map.back()->setTexture(wood_texture3);
    map.back()->resize(glm::vec3(7.5f, 0.0f, 7.5f));

    base.push_back(new Cube());
    base[0]->setTexture(wood_texture4);

    Xhandle.reserve(4);
    Xhandle.push_back(new Cube());
    Xhandle[0]->setTexture(handle);
    Xhandle.push_back(new Cube());
    Xhandle.push_back(new Cube());
    Xhandle.push_back(new Cube());
    for (int i{ 1 }; i < 4; ++i)
    {
        Xhandle[i]->setTexture(handle_bar);
    }

    Zhandle.reserve(4);
    Zhandle.push_back(new Cube());
    Zhandle[0]->setTexture(handle);
    Zhandle.push_back(new Cube()); 
    Zhandle.push_back(new Cube());
    Zhandle.push_back(new Cube());
    for (int i{1}; i < 4; ++i)
    {
        Zhandle[i]->setTexture(handle_bar);
    }

    XaxisFrame.reserve(4);
    ZaxisFrame.reserve(4);
    for (int i{}; i < 4; ++i)
    {
        XaxisFrame.push_back(new Cube());
        XaxisFrame[i]->setTexture(wood_texture2);
        ZaxisFrame.push_back(new Cube());
        ZaxisFrame[i]->setTexture(wood_texture1);
    }

    // 밑바닥 작업
    base[0]->resize(glm::vec3(10.0f, 0.5f, 10.0f));
    base[0]->move(glm::vec3(0.0f, -5.0f, 0.0f));

    // x축 핸들 작업
    Xhandle[0]->resize(glm::vec3(0.5f, 1.0f, 1.0f));
    Xhandle[0]->move(glm::vec3(10.5f, -3.0f, 0.0f));

    Xhandle[1]->resize(glm::vec3(9.4f, 0.2f, 0.2f));
    Xhandle[1]->move(glm::vec3(0.7f, -3.0f, 0.0f));

    Xhandle[2]->resize(glm::vec3(0.2f,0.5f,0.2f));
    Xhandle[3]->resize(glm::vec3(0.2f, 0.5f, 0.2f));

    Xhandle[2]->resize(glm::vec3(0.2f, 1.5f, 0.2f));
    Xhandle[2]->move(glm::vec3(8.5f, -1.5f, 0.0f));
    Xhandle[3]->resize(glm::vec3(0.2f, 1.5f, 0.2f));
    Xhandle[3]->move(glm::vec3(-8.5f, -1.5f, 0.0f));

    // z축 핸들 작업
    Zhandle[0]->resize(glm::vec3(1.0f, 1.0f, 0.5f));
    Zhandle[0]->move(glm::vec3(0.0f, -3.0f, 10.5f));

    Zhandle[1]->resize(glm::vec3(0.2f, 0.2f, 9.7f));
    Zhandle[1]->move(glm::vec3(0.0f, -3.0f, 0.7f));

    Zhandle[2]->resize(glm::vec3(0.2f, 1.5f, 0.2f));
    Zhandle[2]->move(glm::vec3(0.0f, -1.5f, 9.5f));
    Zhandle[3]->resize(glm::vec3(0.2f, 1.5f, 0.2f));
    Zhandle[3]->move(glm::vec3(0.0f, -1.5f, -9.5f));

    // x축 바깥 틀 작업
    XaxisFrame[0]->resize(glm::vec3(0.2f, 1.0f, 8.75f));
    XaxisFrame[0]->move(glm::vec3(8.5f, 0.0f, 0.0f));
    
    XaxisFrame[1]->resize(glm::vec3(0.2f, 1.0f, 8.75f));
    XaxisFrame[1]->move(glm::vec3(-8.5f, 0.0f, 0.0f));

    XaxisFrame[2]->resize(glm::vec3(8.75f, 1.0f, 0.2f));
    XaxisFrame[2]->move(glm::vec3(0.0f, 0.0f, 8.5f));

    XaxisFrame[3]->resize(glm::vec3(8.75f, 1.0f, 0.2f));
    XaxisFrame[3]->move(glm::vec3(0.0f, 0.0f, -8.5f));

    // z축 바깥 틀 작업
    ZaxisFrame[0]->resize(glm::vec3(0.2f, 1.0f, 9.75f));
    ZaxisFrame[0]->move(glm::vec3(9.5f, 0.0f, 0.0f));
    
    ZaxisFrame[1]->resize(glm::vec3(0.2f, 1.0f, 9.75f));
    ZaxisFrame[1]->move(glm::vec3(-9.5f, 0.0f, 0.0f));
    
    ZaxisFrame[2]->resize(glm::vec3(9.75f, 1.0f, 0.2f));
    ZaxisFrame[2]->move(glm::vec3(0.0f, 0.0f, 9.5f));
    
    ZaxisFrame[3]->resize(glm::vec3(9.75f, 1.0f, 0.2f));
    ZaxisFrame[3]->move(glm::vec3(0.0f, 0.0f, -9.5f));

    // 초기 map 상태 저장
    initialMapPos.reserve(map.size());
    initialMapRot.reserve(map.size());
    for (auto* m : map)
    {
        initialMapPos.push_back(m->getPos());
        initialMapRot.push_back(m->getRot());
    }
}

static inline glm::vec3 ApplyParentToPoint(const glm::vec3& p, const glm::quat& rot, const glm::vec3& pivot)
{
    glm::vec3 v = p - pivot;
    glm::vec3 r = rot * v;
    return pivot + r;
}

void Labyrinth::Xrotate(float theta)
{
    double next = static_cast<double>(rotatedAmountX) + static_cast<double>(theta);
    if (next > static_cast<double>(maxRotationX) || next < -static_cast<double>(maxRotationX))
        return;
    rotatedAmountX = static_cast<float>(next);

    glm::quat pitch = glm::angleAxis(glm::radians(rotatedAmountX), glm::vec3(1.0f, 0.0f ,0.0f));
    glm::quat roll  = glm::angleAxis(glm::radians(rotatedAmountZ), glm::vec3(0.0f, 0.0f, 1.0f));
    labyrinthRotation = glm::normalize(pitch * roll);

    // 1) 먼저 정적 벽 재적용 (teleport + setRotationAbsolute)
    for (size_t i = 0; i < map.size(); ++i)
    {
        glm::vec3 newPos = ApplyParentToPoint(initialMapPos[i], labyrinthRotation, pivot);
        map[i]->teleport(newPos);
        map[i]->setRotationAbsolute(glm::normalize(labyrinthRotation * initialMapRot[i]));
    }

    // 2) 핸들과 프레임은 원하는 방식 유지
    for (auto& xf : XaxisFrame)
        xf->rotate(theta, glm::vec3(1.0f, 0.0f, 0.0f), pivot);
    Xhandle[0]->rotate(theta, glm::vec3(1.0f, 0.0f, 0.0f));

    // 3) 이제 동적 오브젝트들의 충돌 처리(Update 루프에서 수행 권장)
    //    - 여기서는 순서만 강조. 실제 동적 오브젝트가 어디에 있는지에 따라 호출 위치 조정.
}

void Labyrinth::Zrotate(float theta)
{
    double next = static_cast<double>(rotatedAmountZ) + static_cast<double>(theta);
    if (next > static_cast<double>(maxRotationZ) || next < -static_cast<double>(maxRotationZ))
        return;

    rotatedAmountZ = static_cast<float>(next);

    glm::quat pitch = glm::angleAxis(glm::radians(rotatedAmountX), glm::vec3(1.0f, 0.0f ,0.0f));
    glm::quat roll  = glm::angleAxis(glm::radians(rotatedAmountZ), glm::vec3(0.0f, 0.0f, 1.0f));
    labyrinthRotation = glm::normalize(pitch * roll);

    for (size_t i = 0; i < map.size(); ++i)
    {
        glm::vec3 newPos = ApplyParentToPoint(initialMapPos[i], labyrinthRotation, pivot);
        map[i]->teleport(newPos);

        glm::quat newRot = glm::normalize(labyrinthRotation * initialMapRot[i]);
        map[i]->setRotationAbsolute(newRot);
    }

    for (auto& zf : ZaxisFrame)
        zf->rotate(theta, glm::vec3(0.0f, 0.0f, 1.0f), pivot);
    Zhandle[0]->rotate(theta, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Labyrinth::draw(GLuint shader)
{
    for (const auto& m : map)
        m->Draw(shader);
    for (const auto& b : base)
        b->Draw(shader);
    for (const auto& xh : Xhandle)
        xh->Draw(shader);
    for (const auto& zh : Zhandle)
        zh->Draw(shader);
    for (const auto& xf : XaxisFrame)
        xf->Draw(shader);
    for (const auto& zf : ZaxisFrame)
        zf->Draw(shader);
}