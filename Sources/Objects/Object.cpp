#include "Object.h"

#include "../Core/Objects/Components/Component.h"

Object::~Object() noexcept
{
    for (std::unique_ptr<Component>& component : components)
    {
        component.reset();
    }
}