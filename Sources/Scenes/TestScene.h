#pragma once

#include "../PCH.h"
#include "../Rendering/Cube.h"
#include "../Rendering/Labyrinth.h"
#include "../Rendering/Light.h"
#include "../Rendering/Camera.h"
#include "../Rendering/TextureLoader.h"

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
    Labyrinth* labyrinth;
    Light*	light;

    GLsizei width, height, numberOfChannel;
    GLuint  wood_texture1 = TextureLoader::LoadTexture("Sources/Rendering/wood_texture1.png", width, height, numberOfChannel);
    GLuint  wood_texture2 = TextureLoader::LoadTexture("Sources/Rendering/wood_texture2.png", width, height, numberOfChannel);
    GLuint  wood_texture3 = TextureLoader::LoadTexture("Sources/Rendering/wood_texture3.png", width, height, numberOfChannel);
    GLuint  wood_texture4 = TextureLoader::LoadTexture("Sources/Rendering/wood_texture4.png", width, height, numberOfChannel);
    GLuint  handle = TextureLoader::LoadTexture("Sources/Rendering/handle.png", width, height, numberOfChannel);
    GLuint  handle_bar = TextureLoader::LoadTexture("Sources/Rendering/handle_bar.png", width, height, numberOfChannel);
};