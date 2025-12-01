#pragma once

#include "../Framework/Objects.h"
#include "../Framework/Resources.h"

class OBB : public Component
{
public:
    explicit OBB(Object* const owner);
    virtual ~OBB() noexcept;

    const glm::vec3 getCenter()
    {
        return pos;
    }
    const glm::vec3 getHalf()
    {
        return radius;
    }
    const glm::mat3 getRot()
    {
        return glm::mat3_cast(rotation);
    }
    const glm::vec3 getNormal()
    {
        return glm::mat3_cast(rotation)[1];
    }

    void move(glm::vec3 v)
    {
        pos += v;
    }

    void teleport(glm::vec3 v)
    {
        pos = v;
    }

    void resize(glm::vec3 v)
    {
        radius = v;
    }

    void rotate(float theta, glm::vec3 axis)
    {
        glm::quat q = glm::angleAxis(glm::radians(theta), glm::normalize(axis));
        rotation    = q * rotation;
    }

    glm::vec3 safeNormalize(const glm::vec3& v)
    {
        float len = glm::length(v);
        if (len < 1e-8f)
            return glm::vec3(0.0f);
        return v / len;
    }

    float projectedRadius(const glm::vec3& axis)
    {
        glm::mat3 rotMat = glm::mat3_cast(rotation);
        return radius.x * fabs(glm::dot(axis, rotMat[0])) + radius.y * fabs(glm::dot(axis, rotMat[1])) +
                radius.z * fabs(glm::dot(axis, rotMat[2]));
    }

    bool testOBBOBB_SAT(const OBB& target, glm::vec3& outMTV)
    {
        const float EPS        = 1e-6f;
        float       minOverlap = std::numeric_limits<float>::infinity();
        glm::vec3   smallestAxis(0.0f);

        glm::vec3 d = pos - target.pos;

        glm::mat3 rotMat    = glm::mat3_cast(rotation);
        glm::mat3 targetRot = glm::mat3_cast(target.rotation);

        std::vector<glm::vec3> axes;
        axes.reserve(15);
        for (int i{}; i < 3; ++i)
            axes.push_back(targetRot[i]);
        for (int i{}; i < 3; ++i)
            axes.push_back(rotMat[i]);
        for (int i{}; i < 3; ++i)
        {
            for (int j{}; j < 3; ++j)
            {
                glm::vec3 ax = glm::cross(targetRot[i], rotMat[j]);
                if (glm::length(ax) > EPS)
                {
                    axes.push_back(safeNormalize(ax));
                }
            }
        }

        for (const glm::vec3& rawAxis : axes)
        {
            glm::vec3 axis = safeNormalize(rawAxis);
            if (glm::length(axis) < EPS)
                continue;

            float rA = target.radius.x * fabs(glm::dot(axis, targetRot[0])) +
                    target.radius.y * fabs(glm::dot(axis, targetRot[1])) +
                    target.radius.z * fabs(glm::dot(axis, targetRot[2]));
            float rB      = projectedRadius(axis);
            float dist    = fabs(glm::dot(axis, d));
            float overlap = (rA + rB) - dist;

            if (overlap < 0.0f)
            {
                outMTV = glm::vec3(0.0f);
                return false;
            }

            if (overlap < minOverlap)
            {
                minOverlap   = overlap;
                smallestAxis = axis;
                if (glm::dot(axis, d) < 0.0f)
                    smallestAxis = -smallestAxis;
            }
        }

        outMTV = smallestAxis * minOverlap;
        return true;
    }

private:
    glm::vec3 pos;

    glm::vec3 radius;

    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    GLuint vao, vbo, ebo;

    const glm::vec3 cubeVertices[8] = {{-1.0f, -1.0f, -1.0f},
                                       {1.0f, -1.0f, -1.0f},
                                       {1.0f, 1.0f, -1.0f},
                                       {-1.0f, 1.0f, -1.0f},
                                       {-1.0f, -1.0f, 1.0f},
                                       {1.0f, -1.0f, 1.0f},
                                       {1.0f, 1.0f, 1.0f},
                                       {-1.0f, 1.0f, 1.0f}};

    const unsigned int cubeEdges[12][2] = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};
};
