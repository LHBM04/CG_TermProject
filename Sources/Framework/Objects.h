#pragma once

#include "Common.h"

class Component;
class Object;
class Transform;

/**
 * @brief 지정한 타입이 Component에서 파생되었는지 여부를 검사합니다.
 */
template <typename TComponent>
concept FromComponent = std::derived_from<TComponent, Component>;

/**
 * @brief 지정한 타입이 Object에서 파생되었는지 여부를 검사합니다.
 */
template <typename TObject>
concept FromObject = std::derived_from<TObject, Object>;

/**
 * @class Object
 *
 * @brief 오브젝트를 정의합니다.
 */
class Object
{
public:
    /**
     * @brief 생성자.
     *
     * @param name_ 생성할 오브젝트의 이름
     * @param tag_  생성할 오브젝트의 태그
     */
    Object(std::string_view name_ = "New Object", std::string_view tag_ = "Not tag") noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Object() noexcept;

    /**
     * @brief 해당 오브젝트를 초기화합니다.
     */
    void Start() noexcept;

    /**
     * @brief 해당 오브젝트를 매 프레임마다 갱신합니다.
     */
    void Update() noexcept;

    /**
     * @brief 해당 오브젝트를 고정 프레임마다 갱신합니다.
     */
    void FixedUpdate() noexcept;

    /**
     * @brief 해당 오브젝트를 렌더링합니다.
     */
    void Render() noexcept;

    /**
     * @brief 해당 오브젝트의 이름을 가져옵니다.
     *
     * @return String 해당 오브젝트의 이름
     */
    [[nodiscard]]
    inline std::string GetName() const noexcept
    {
        return name;
    }

    /**
     * @brief 해당 오브젝트의 이름을 지정한 이름으로 변경합니다.
     *
     * @param name_ 지정할 이름
     */
    inline void SetName(std::string_view name_) noexcept
    {
        name = name_;
    }

    /**
     * @brief 해당 오브젝트의 태그를 가져옵니다.
     *
     * @return String 해당 오브젝트의 태그
     */
    [[nodiscard]]
    inline std::string GetTag() const noexcept
    {
        return tag;
    }

    /**
     * @brief 해당 오브젝트의 태그를 지정한 태그로 변경합니다.
     *
     * @param tag_ 지정할 태그
     */
    inline void SetTag(std::string_view tag_) noexcept
    {
        tag = tag_;
    }

    /**
     * @brief 해당 오브젝트의 활성화 여부를 반환합니다.
     *
     * @return bool 해당 오브젝트의 활성화 여부
     */
    [[nodiscard]]
    inline bool IsEnabled() const noexcept
    {
        return isEnabled;
    }

    /**
     * @brief 해당 오브젝트의 활성화 여부를 설정합니다.
     *
     * @param enabled_ 해당 오브젝트의 활성화 여부
     */
    inline void SetEnabled(bool enabled_) noexcept
    {
        isEnabled = enabled_;
    }

    /**
     * @brief 해당 오브젝트의 파괴 여부를 반환합니다.
     *
     * @return bool 해당 오브젝트의 파괴 여부
     */
    [[nodiscard]]
    inline bool IsDestroyed() const noexcept
    {
        return isDestroyed;
    }

    /**
     * @brief 해당 오브젝트를 파괴합니다.
     */
    inline void Destroy() noexcept
    {
        isDestroyed = true;
    }

    /**
     * @brief 해당 오브젝트의 트랜스폼 컴포넌트를 반환합니다.
     *
     * @return Transform* 해당 오브젝트의 트랜스폼 컴포넌트
     */
    [[nodiscard]]
    inline Transform* const GetTransform() const noexcept
    {
        return transform;
    }

    /**
     * @brief 지정한 타입의 컴포넌트가 있는지 여부를 반환합니다.
     *
     * @tparam TComponent 지정할 타입
     *
     * @return bool 지정한 타입의 컴포넌트가 있는지 여부
     */
    template <FromComponent TComponent>
    bool HasComponent() const noexcept
    {
        return GetComponent<TComponent>() != nullptr;
    }

    /**
     * @brief 지정한 타입의 컴포넌트를 추가합니다.
     *
     * @tparam TComponent 지정할 타입
     *
     * @return TComponent* 추가된 컴포넌트
     */
    template <FromComponent TComponent>
    TComponent* AddComponent() noexcept
    {
        if (HasComponent<TComponent>())
        {
            return nullptr;
        }

        std::unique_ptr<TComponent> newComponent = std::make_unique<TComponent>(this);
        newComponent->Awake();

        return dynamic_cast<TComponent*>(components.emplace_back(std::move(newComponent)).get());
    }

    /**
     * @brief 지정한 타입의 컴포넌트를 파괴합니다.
     *
     * @tparam TComponent 지정할 타입
     */
    template <FromComponent TComponent>
    void DestroyComponent() noexcept
    {
        TComponent* component = GetComponent<TComponent>();

        if (component != nullptr)
        {
            component->Destroy();
        }
    }

    /**
     * @brief 지정한 타입의 컴포넌트를 반환합니다.
     *
     * @tparam TComponent 지정할 타입
     *
     * @return TComponent* 지정한 타입의 컴포넌트
     */
    template <FromComponent TComponent>
    TComponent* GetComponent() const noexcept
    {
        const std::type_index targetType = std::type_index(typeid(TComponent));

        for (const std::unique_ptr<Component>& component : components)
        {
            if (std::type_index(typeid(*component)) == targetType)
            {
                return static_cast<TComponent*>(component.get());
            }
        }

        return nullptr;
    }

protected:
    /**
     * @brief 해당 오브젝트의 이름.
     */
    std::string name;

    /**
     * @brief 해당 오브젝트의 태그.
     */
    std::string tag;

    /**
     * @brief 해당 오브젝트의 트랜스폼 컴포넌트.
     */
    Transform* transform;

    /**
     * @brief 해당 오브젝트에 추가된 컴포넌트들.
     */
    std::vector<std::unique_ptr<Component>> components;

private:
    /**
     * @brief 해당 오브젝트의 활성화 여부.
     */
    bool isEnabled;

    /**
     * @brief 해당 오브젝트의 파괴 여부.
     */
    bool isDestroyed;
};

/**
 * @class Component
 *
 * @brief 컴포넌트를 정의합니다.
 */
class Component
{
public:
    /**
     * @brief 생성자.
     * 
     * @param owner_ 해당 컴포넌트의 오너 엔티티
     */
    Component(Object* const owner_);

    /**
     * @brief 소멸자.
     */
    virtual ~Component() noexcept;

    /**
     * @brief 해당 컴포넌트가 생성될 때 호출됩니다.
     */
    virtual void Awake()
    {
    }

    /**
     * @brief 해당 컴포넌트가 활성화될 때 호출됩니다.
     */
    virtual void OnEnabled()
    {
    }

    /**
     * @brief 활성화된 상태에서 첫 번째 Update 실행 직전에 호출됩니다.
     */
    virtual void Start()
    {
    }

    /**
     * @brief 매 프레임마다 호출됩니다.
     */
    virtual void Update()
    {
    }

    /**
     * @brief 고정된 시간 간격마다 호출됩니다. (물리 연산 등에 사용)
     */
    virtual void FixedUpdate()
    {
    }

    /**
     * @brief 모든 Update가 끝난 후 호출됩니다. (카메라 이동 등에 사용)
     */
    virtual void LateUpdate()
    {
    }

    /**
     * @brief 렌더링 시 호출됩니다.
     */
    virtual void Render()
    {
    }
    
    /**
     * @brief 해당 컴포넌트가 비활성화될 때 호출됩니다.
     */
    virtual void OnDisabled()
    {
    }

    /**
     * @brief 컴포넌트가 파괴될 때 호출됩니다.
     */
    virtual void OnDestroy()
    {
    }

    /**
     * @brief 해당 컴포넌트의 오너 엔티티를 반환합니다.
     *
     * @return Object* 해당 컴포넌트의 오너 엔티티
     */
    [[nodiscard]]
    inline Object* GetOwner() const noexcept
    {
        return owner;
    }

    /**
     * @brief 해당 오브젝트의 트랜스폼 컴포넌트를 반환합니다.
     *
     * @return Transform* 해당 오브젝트의 트랜스폼 컴포넌트
     */
    [[nodiscard]]
    inline Transform* const GetTransform() const noexcept
    {
        return owner->GetTransform();
    }

    /**
     * @brief 컴포넌트의 활성화 여부를 반환합니다.
     *
     * @return bool 해당 컴포넌트의 활성화 여부
     */
    [[nodiscard]]
    inline bool IsEnabled() const noexcept
    {
        return isEnabled;
    }

    /**
     * @brief 컴포넌트의 활성화 여부를 설정합니다.
     *
     * @param enabled_ 해당 컴포넌트의 활성화 여부
     */
    inline void SetEnabled(bool enabled_) noexcept
    {
        isEnabled = enabled_;

        if (isEnabled)
        {
            OnEnabled();
        }
        else
        {
            OnDisabled();
        }
    }

    /**
     * @brief 해당 컴포넌트가 Start되었는지 여부를 반환합니다.
     *
     * @return bool 해당 컴포넌트가 Start되었는지 여부
     */
    [[nodiscard]]
    inline bool IsStarted() const noexcept
    {
        return isStarted;
    }

    /**
     * @brief 해당 컴포넌트가 Start되었음을 설정합니다.
     */
    inline void SetStarted() noexcept
    {
        isStarted = true; 
    }

    /**
     * @brief 해당 컴포넌트를 파괴해야 하는지 여부를 반환합니다.
     *
     * @bool 해당 컴포넌트를 파괴해야 하는지 여부
     */
    [[nodiscard]]
    inline bool IsDestroyed() noexcept
    {
        return isDestroyed;
    }

    /**
     * @brief 해당 컴포넌트를 파괴합니다.
     */
    inline void Destroy() noexcept
    {
        isDestroyed = true;
    }

protected:
    /**
     * @brief 해당 컴포넌트의 오너.
     */
    Object* owner;

    /**
     * @brief 해당 컴포넌트의 트랜스폼 컴포넌트.
     */
    Transform* transform;

private:
    /**
     * @brief 해당 컴포넌트의 활성화 여부.
     */
    bool isEnabled;

    /**
     * @brief 해당 컴포넌트가 시작되었는지 여부.
     */
    bool isStarted;

    /**
     * @brief 해당 컴포넌트의 파괴 여부.
     */
    bool isDestroyed;
};

/**
 * @class Transform
 *
 * @brief 오브젝트의 위치, 회전 및 크기를 정의합니다.
 */
class Transform : public Component
{
public:
    /**
     * @brief 생성자.
     * 
     * @param owner 해당 컴포넌트의 오너
     */
    explicit Transform(Object* const owner);

    /**
     * @brief 소멸자.
     */
    virtual ~Transform() noexcept;

    /**
     * @brief 오브젝트의 위치 값을 반환합니다.
     *
     * @return glm::fvec3 오브젝트의 위치 값
     */
    [[nodiscard]]
    inline const glm::fvec3& GetPosition() const noexcept
    {
        return position;
    }

    /**
     * @brief 오브젝트의 위치 값을 설정합니다.
     *
     * @param position_ 설정할 위치 값
     */
    inline void SetPosition(const glm::fvec3& position_) noexcept
    {
        position = position_;
    }

    /**
     * @brief 오브젝트의 회전 값을 반환합니다.
     *
     * @return glm::fvec3 오브젝트의 회전 값
     */
    [[nodiscard]]
    inline const glm::fvec3& GetRotation() const noexcept
    {
        return rotation;
    }

    /**
     * @brief 오브젝트의 회전 값을 설정합니다.
     *
     * @param rotation_ 설정할 회전 값
     */
    inline void SetRotation(const glm::fvec3& rotation_) noexcept
    {
        rotation = rotation_;
    }

    /**
     * @brief 오브젝트의 크기 값을 반환합니다.
     *
     * @return glm::fvec3 오브젝트의 크기 값
     */
    [[nodiscard]]
    inline const glm::fvec3& GetScale() const noexcept
    {
        return scale;
    }

    /**
     * @brief 오브젝트의 크기 값을 설정합니다.
     *
     * @param scale_ 설정할 크기 값
     */
    inline void SetScale(const glm::fvec3& scale_) noexcept
    {
        scale = scale_;
    }

    /**
     * @brief 해당 트랜스폼의 부모 트랜스폼을 반환합니다.
     *
     * @return Transform* 해당 트랜스폼의 부모 트랜스폼
     */
    [[nodiscard]]
    inline Transform* const GetParent() const noexcept
    {
        return parent;
    }

    /**
     * @brief 해당 트랜스폼의 부모 트랜스폼을 설정합니다.
     *
     * @param parent_ 설정할 부모 트랜스폼
     */
    inline void SetParent(Transform* const parent_) noexcept
    {
        parent = parent_;
    }

    /**
     * @brief 로컬 좌표계에서의 변환 행렬을 반환합니다.
     * 
     * @return glm::fmat4x4 로컬 좌표계에서의 변환 행렬
     */
    [[nodiscard]]
    inline glm::fmat4x4 GetLocalMatrix() const noexcept
    {
        glm::fmat4x4 model(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::fvec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::fvec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::fvec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);

        return model;
    }

    /**
     * @brief 월드 좌표계에서의 변환 행렬을 반환합니다.
     *
     * @return glm::fmat4x4 월드 좌표계에서의 변환 행렬
     */
    [[nodiscard]]
    inline glm::fmat4x4 GetWorldMatrix() const noexcept
    {
        glm::fmat4x4 localMatrix = GetLocalMatrix();

        if (parent != nullptr)
        {
            return parent->GetWorldMatrix() * localMatrix;
        }
        else
        {
            return localMatrix;
        }
    }

private:
    /**
     * @brief 오브젝트의 위치 값.
     */
    glm::fvec3 position;

    /**
     * @brief 오브젝트의 회전 값.
     */
    glm::fvec3 rotation;

    /**
     * @brief 오브젝트의 크기 값.
     */
    glm::fvec3 scale;

    /**
     * @brief 해당 트랜스폼의 부모 트랜스폼.
     */
    Transform* parent;
};