#pragma once

#include "../PCH.h"
#include "../Rendering/Cube.h"
#include "../Rendering/Light.h"
#include "../Rendering/Camera.h"

#include "Scene.h"

/**
 * @class TestScene
 * 
 * @brief 테스트용 씬을 정의합니다.
 */
class TestScene final
	: public Scene
{
public:
	/**
	 * @brief 생성자.
	 */
	TestScene() noexcept;
    void make_vertexShaders();
    void make_fragmentShaders();
    void make_shaderProgram();
    char* filetobuf(const char* file);

	/**
	 * @brief 소멸자.
	 */
    virtual ~TestScene() noexcept override;

protected:
    virtual void OnEnter() noexcept override;
	virtual void OnUpdate() noexcept override;
	virtual void OnRender() noexcept override;
	virtual void OnExit() noexcept override;

private:
    GLuint  shaderProgramID;
    GLchar *vertexSource, *fragmentSource;
    GLuint  vertexShader, fragmentShader;

	Vector2 mousePos;
    float   scrollOffset;

	Camera* cam;
    Cube*	cube;
    std::vector<Cube*> map;
    Light*	light;
};