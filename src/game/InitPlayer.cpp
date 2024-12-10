#include "InitPlayer.hpp"

#include "SoftBodyBox.hpp"

#include "Transform.hpp"
#include "BoxCollider3D.hpp"
#include "PlayerTag.hpp"

void Game::InitPlayer(ES::Engine::Registry& registry)
{
    auto player = Objects::CreateSoftBodyBox(registry, glm::vec3(-2, 7, 2), glm::vec3(1, 1, 1));
    registry.GetRegistry().emplace<Game::Player>(player);
}
