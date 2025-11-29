#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using FVector2 = glm::vec2;
using IVector2 = glm::ivec2;

using FVector3 = glm::vec3;
using IVector3 = glm::ivec3;

using FVector4 = glm::vec4;
using IVector4 = glm::ivec4;

using FMatrix4 = glm::mat4;

using RGB  = glm::vec3;
using RGBA = glm::vec4;

/**
 * @class Math
 * 
 * @brief 
 */
namespace Math
{
    /**
     * @brief
     */
    constexpr f32 PI = 3.14159265359f;

    /**
     * @brief
     */
    constexpr f32 DEG2RAD = PI / 180.0f;

    /**
     * @brief
     */
    constexpr f32 RAD2DEG = 180.0f / PI;

    [[nodiscard]]
    inline constexpr f32 ToRadians(f32 degrees_) noexcept
    {
        return degrees_ * DEG2RAD;
    }

    [[nodiscard]]
    inline constexpr f32 ToDegrees(f32 radians_) noexcept
    {
        return radians_ * RAD2DEG;
    }
}