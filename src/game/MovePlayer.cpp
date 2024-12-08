#include "MovePlayer.hpp"
#include "PlayerTag.hpp"

#include "SoftBodyBox.hpp"
#include "SoftBodyNode.hpp"

static glm::vec3 GetPlayerMovement()
{
    constexpr static float MOVEMENT_FORCE = 20.f;
    glm::vec3 movement = {0, 0, 0};

    if (Raylib::IsKeyDown(Raylib::KEY_W))
    {
        movement.z += MOVEMENT_FORCE;
    }
    if (Raylib::IsKeyDown(Raylib::KEY_S))
    {
        movement.z -= MOVEMENT_FORCE;
    }
    if (Raylib::IsKeyDown(Raylib::KEY_A))
    {
        movement.x += MOVEMENT_FORCE;
    }
    if (Raylib::IsKeyDown(Raylib::KEY_D))
    {
        movement.x -= MOVEMENT_FORCE;
    }

    return movement;
}

void Game::MovePlayer(ES::Engine::Registry& registry)
{
    auto movement = GetPlayerMovement();
    auto view = registry.GetRegistry().view<Game::PlayerTag, Objects::SoftBodyBox>();

    for (auto entity : view)
    {
        auto &box = view.get<Objects::SoftBodyBox>(entity);

        for (auto particle : box.entities)
        {
            auto &node = registry.GetRegistry().get<ES::Plugin::Physics::Component::SoftBodyNode>(particle);
            node.ApplyForce(movement);
        }
    }
}