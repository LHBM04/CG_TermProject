#pragma once

#include "../PCH.h"

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
    /**
     * @brief 테스트용 오브젝트.
     */
    GLUquadric *testObject;
};