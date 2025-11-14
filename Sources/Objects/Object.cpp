#include "Object.h"

#include "../Component.h"

Object::~Object() noexcept
{
    for (std::unique_ptr<Component>& component : components)
    {
        component.reset();
    }
}