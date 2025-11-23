#pragma once

#include "../../Framework/Scene/Scene.h"

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
    /**
     * @brief 
     */
    virtual void OnEnter() noexcept override;

	/**
	 * @brief 
	 */
	virtual void OnUpdate() noexcept override;

	/**
	 * @brief 
	 */
	virtual void OnRender() noexcept override;

	/**
	 * @brief 
	 */
	virtual void OnExit() noexcept override;
};