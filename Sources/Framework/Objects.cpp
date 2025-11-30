#include "Objects.h"

Component::Component(Object* const owner_) 
    : owner(owner_)
    , transform(owner_->GetTransform())
    , isEnabled(true)
    , isStarted(false)
    , isDestroyed(false)
{
}

Component::~Component() noexcept
{
}

Object::Object(std::string_view name_, std::string_view tag_) noexcept
    : name(name_)
    , tag(tag_)
    , isEnabled(true)
    , isDestroyed(false)
{
    // 기본 컴포넌트로 Transform 추가
    transform = AddComponent<Transform>();
}

Object::~Object() noexcept
{
    for (auto& component : components)
    {
        component->Destroy();
    }
}

void Object::Start() noexcept
{
    if (!isEnabled)
    {
        return;
    }

    for (const std::unique_ptr<Component>& component : components)
    {
        if (component->IsEnabled() && !component->IsDestroyed())
        {
            component->Start();
            component->SetStarted();
        }
    }
}

void Object::Update() noexcept
{
    if (!isEnabled)
    {
        return;
    }

    for (const std::unique_ptr<Component>& component : components)
    {
        if (component->IsEnabled() && !component->IsDestroyed())
        {
            if (!component->IsStarted())
            {
                component->Start();
                component->SetStarted();
            }

            component->Update();
            component->LateUpdate();
        }
    }
}

void Object::FixedUpdate() noexcept
{
    if (!isEnabled)
    {
        return;
    }

    for (const std::unique_ptr<Component>& component : components)
    {
        if (component->IsEnabled() && !component->IsDestroyed())
        {
            component->FixedUpdate();
        }
    }
}

void Object::Render() noexcept
{
    if (!isEnabled)
    {
        return;
    }

    for (const std::unique_ptr<Component>& component : components)
    {
        if (component->IsEnabled() && !component->IsDestroyed())
        {
            component->Render();
        }
    }
}

Transform::Transform(Object* const owner)
    : Component(owner)
    , position(0.0f, 0.0f, 0.0f)
    , rotation(0.0f, 0.0f, 0.0f)
    , scale(1.0f, 1.0f, 1.0f)
{
}

Transform::~Transform() noexcept
{
}