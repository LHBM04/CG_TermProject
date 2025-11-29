#include "Objects.h"

Object::Object(StringView name_, 
               StringView tag_) noexcept
    : name(name_)
    , tag(tag_)
    , isEnabled(true)
{
}

Object::~Object() noexcept
{
    for (auto& component : components)
    {
        component->OnDestroy();
    }
}

void Object::Update() noexcept
{
    if (!isEnabled)
    {
        return;
    }

    for (auto& component : components)
    {
        if (component->IsEnabled() && !component->IsDestroyed())
        {
            component->OnUpdate();
        }
    }
}

void Object::FixedUpdate() noexcept
{
    if (!isEnabled)
    {
        return;
    }

    for (auto& component : components)
    {
        if (component->IsEnabled() && !component->IsDestroyed())
        {
            component->OnFixedUpdate();
        }
    }
}

void Object::LateUpdate() noexcept
{
    if (!isEnabled)
    {
        return;
    }

    for (auto& component : components)
    {
        if (component->IsEnabled() && !component->IsDestroyed())
        {
            component->OnLateUpdate();
        }
    }
}

void Object::Render() noexcept
{
    if (!isEnabled)
    {
        return;
    }

    for (auto& component : components)
    {
        if (component->IsEnabled() && !component->IsDestroyed())
        {
            component->OnRender();
        }
    }
}