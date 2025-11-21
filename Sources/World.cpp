#include "World.h"

#include "Transform.h"

void World::Update() noexcept
{

}

void World::Render() noexcept
{
    auto view = registry.view<Transform>();
    
    for (auto entity : view)
    {
        auto& transform = view.get<Transform>(entity);
    }
}