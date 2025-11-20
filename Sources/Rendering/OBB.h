#pragma once

#include "../PCH.h"

class OBB
{
public:
    OBB(glm::vec3 center_, glm::vec3 half_, glm::mat3 rot_)
        : pos(center_)
        , radius(half_)
        , rot(rot_)
    {
        initWireCube();
    }

    void initWireCube()
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeEdges), cubeEdges, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

        glBindVertexArray(0);
    }

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
        return rot;
    }

    void move(glm::vec3 v)
    {
        pos += v;
    }

    void teleport(glm::vec3 v)
    {
        pos = v;
    }

    void rotate(float theta, glm::vec3 axis)
    {
        glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(theta), axis);
        rot = glm::mat3(rotMat) * rot;
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
        return radius.x * fabs(glm::dot(axis, rot[0]))
            + radius.y * fabs(glm::dot(axis, rot[1]))
            + radius.z * fabs(glm::dot(axis, rot[2]));
    }

    bool testOBBOBB_SAT(const OBB& target, glm::vec3& outMTV)
    {
        const float EPS = 1e-6f;
        float       minOverlap = std::numeric_limits<float>::infinity();
        glm::vec3   smallestAxis(0.0f);

        glm::vec3 d = pos - target.pos;

        std::vector<glm::vec3> axes;
        axes.reserve(15);
        for (int i{}; i < 3; ++i)
            axes.push_back(target.rot[i]);
        for (int i{}; i < 3; ++i)
            axes.push_back(rot[i]);
        for (int i{}; i < 3; ++i)
        {
            for (int j{}; j < 3; ++j)
            {
                glm::vec3 ax = glm::cross(target.rot[i], rot[j]);
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

            float rA = target.radius.x * fabs(glm::dot(axis, target.rot[0])) 
                + target.radius.y * fabs(glm::dot(axis, target.rot[1])) 
                + target.radius.z * fabs(glm::dot(axis, target.rot[2]));
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

    void draw(GLuint shaderProgram)
    {
        glUseProgram(shaderProgram);

        glm::mat4 rot4(1.0f);
        rot4[0] = glm::vec4(rot[0], 0.0f);
        rot4[1] = glm::vec4(rot[1], 0.0f);
        rot4[2] = glm::vec4(rot[2], 0.0f);

        glm::mat4 model =
                glm::translate(glm::mat4(1.0f), pos) * rot4 * glm::scale(glm::mat4(1.0f), radius * 2.0f);
        GLuint    modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(vao);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    }

private: 
    glm::vec3 pos;
    glm::vec3 radius;
    glm::mat3 rot;

    GLuint vao, vbo, ebo;

    const glm::vec3 cubeVertices[8] =
    {
            {-0.5f, -0.5f, -0.5f},
            {0.5f, -0.5f, -0.5f},
            {0.5f, 0.5f, -0.5f},
            {-0.5f, 0.5f, -0.5f},
            {-0.5f, -0.5f, 0.5f},
            {0.5f, -0.5f, 0.5f},
            {0.5f, 0.5f, 0.5f},
            {-0.5f, 0.5f, 0.5f},
    };

    const unsigned int cubeEdges[12][2] =
    {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };
};

