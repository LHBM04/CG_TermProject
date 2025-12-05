#pragma once

namespace Graphics
{
    /**
     * @brief 그래픽스 API를 초기화합니다.
     * @return 
     */
    bool Initialize();

    /**
     * @brief 디버그 메시지 콜백 함수.
     */
    void OnDebugMessage(
        const GLenum  source_,
        const GLenum  type_,
        const GLuint  id_,
        const GLenum  severity_,
        const GLsizei length_,
        const GLchar* message_,
        const GLvoid* userParam_) noexcept;
} // namespace Graphics