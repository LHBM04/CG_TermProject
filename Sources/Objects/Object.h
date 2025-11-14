#pragma once

#include "../PCH.h"

class Component;

/**
 * @class Object
 * 
 * @brief 애플리케이션 내 모든 객체의 기본 단위를 정의합니다.
 */
class Object
{
public:
    /**
     * @brief 소멸자.
     */
    virtual ~Object() noexcept;

    /**
     * @brief 해당 오브젝트로부터 지정한 컴포넌트를 반환합니다.
     * 
     * @tparam TComponent 반환할 컴포넌트 타입
     * 
     * @return Component* const 반환된 컴포넌트
     */
    template <typename TComponent>
        requires std::is_base_of_v<Component, TComponent>
    TComponent* const GetComponent() const noexcept;

    /**
     * @brief 오브젝트에 지정한 컴포넌트를 추가합니다.
     * 
     * @tparam TComponent 추가할 컴포넌트 타입
     * @tparam TArgs      컴포넌트 생성자에 전달할 인자
     * 
     * @return TComponent* const 추가된 컴포넌트
     */
    template <typename TComponent, typename... TArgs>
        requires std::is_base_of_v<Component, TComponent>
    TComponent* const AddComponent(TArgs&&... args_) noexcept;

private:
    /**
     * @brief 해당 객체가 가지고 있는 컴포넌트들.
     */
    std::vector<std::unique_ptr<Component>> components;
};

template<typename TComponent, typename... TArgs>
    requires std::is_base_of_v<Component, TComponent>
TComponent* const Object::AddComponent(TArgs&&... args_) noexcept
{
    std::unique_ptr<TComponent> component = std::make_unique<TComponent>(std::forward<TArgs>(args_)...);
    return components.emplace_back(std::move(component)).get();
}

template <typename TComponent>
    requires std::is_base_of_v<Component, TComponent>
TComponent* const Object::GetComponent() const noexcept
{
    const std::type_index targetType(TComponent);
    for (const std::unique_ptr<TComponent>& component : components)
    {
        std::type_index componentIndex(*(component));
        if (componentIndex == targetType)
        {
            return component.get();
        }
    }

    return nullptr;
}