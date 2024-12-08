#include "InitTerrain.hpp"

#include "SoftBodyBox.hpp"

#include "Transform.hpp"
#include "BoxCollider3D.hpp"

void Game::InitTerrain(ES::Engine::Registry& registry)
{
    ES::Engine::Entity ground = registry.CreateEntity();
    registry.GetRegistry().emplace<ES::Plugin::Object::Component::Transform>(ground, glm::vec3(0, 0, 0));
    registry.GetRegistry().emplace<ES::Plugin::Physics::Component::BoxCollider3D>(ground, glm::vec3(20, 1, 20));
}
