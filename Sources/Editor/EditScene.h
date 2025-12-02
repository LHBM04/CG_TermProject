#pragma once

class Camera;

/**
 * @class EditScene
 *
 * @brief 에디터 씬을 정의합니다.
 */
class EditScene: public Scene
{
public:
	/**
	 * @brief 생성자.
	 */
	explicit EditScene() noexcept;

	/**
	 * @brief 소멸자.
	 */
	virtual ~EditScene() noexcept override;

protected:
    /**
     * @brief 해당 씬에 입장할 때 호출됩니다.
     */
    virtual void OnEnter() noexcept override;

	/**
	 * @brief 해당 씬을 렌더링할 때 호출됩니다.
	 */
	virtual void OnRender() noexcept override;

	/**
	 * @brief 해당 씬에서 퇴장할 때 호출됩니다.
     */
	virtual void OnExit() noexcept override;

private:
    /**
     * @brief 메인 카메라.
     */
    Camera* mainCamera;
};
