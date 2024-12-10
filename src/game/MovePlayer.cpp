#include "MovePlayer.hpp"
#include "PlayerTag.hpp"

#include "SoftBodyBox.hpp"
#include "SoftBodyNode.hpp"

static glm::vec3 GetPlayerMovement(const glm::vec3& cameraForward, const glm::vec3& cameraRight)
{
    constexpr static float MOVEMENT_FORCE = 10.f;
    glm::vec3 movement = {0, 0, 0};

    if (Raylib::IsKeyDown(Raylib::KEY_W))
    {
        movement += cameraForward * MOVEMENT_FORCE;
    }
    if (Raylib::IsKeyDown(Raylib::KEY_S))
    {
        movement -= cameraForward * MOVEMENT_FORCE;
    }
    if (Raylib::IsKeyDown(Raylib::KEY_A))
    {
        movement -= cameraRight * MOVEMENT_FORCE;
    }
    if (Raylib::IsKeyDown(Raylib::KEY_D))
    {
        movement += cameraRight * MOVEMENT_FORCE;
    }

    return movement;
}

void Game::MovePlayer(ES::Engine::Registry& registry)
{
    auto cameraView = registry.GetRegistry().view<Raylib::Camera3D>();

    if (cameraView.size() != 1)
    {
        return;
    }

    glm::vec3 cameraPosition;
    glm::vec3 cameraForward;
    glm::vec3 cameraRight;

    for (auto entity : cameraView)
    {
        auto &camera = cameraView.get<Raylib::Camera3D>(entity);
        cameraPosition = {camera.position.x, camera.position.y, camera.position.z};

        glm::vec3 cameraLookAt = {camera.target.x, camera.target.y, camera.target.z};
        glm::vec3 fullForward = glm::normalize(cameraLookAt - cameraPosition);

        cameraForward = glm::normalize(glm::vec3(fullForward.x, 0.f, fullForward.z));

        glm::vec3 worldUp = {0.f, 1.f, 0.f};
        cameraRight = glm::normalize(glm::cross(cameraForward, worldUp));
    }

    auto movement = GetPlayerMovement(cameraForward, cameraRight);

    if (Raylib::IsKeyPressed(Raylib::KEY_SPACE)) {
        auto view = registry.GetRegistry().view<Game::Player>();

        for (auto entity : view)
        {
            auto &player = view.get<Game::Player>(entity);
            if (player.CanJump()) {
                player.Jump();
                movement.y = 800.f;
            }
        }
    }

    auto view = registry.GetRegistry().view<Game::Player, Objects::SoftBodyBox>();

    // TODO: replace with a max speed component or add it to player
    constexpr float MAX_XZ_SPEED = 10.f;
    // TODO: replace with a max speed component or add it to player
    constexpr float MAX_Y_SPEED = 10.f;

    for (auto entity : view)
    {
        auto &box = view.get<Objects::SoftBodyBox>(entity);

        for (auto particle : box.entities)
        {
            auto &node = registry.GetRegistry().get<ES::Plugin::Physics::Component::SoftBodyNode>(particle);
            node.ApplyForce(movement);

            glm::vec3 xzSpeed = glm::vec3(node.velocity.x, 0, node.velocity.z);

            if (glm::length(xzSpeed) > MAX_XZ_SPEED)
            {
                xzSpeed = glm::normalize(xzSpeed) * MAX_XZ_SPEED;
            }

            // if (glm::abs(node.velocity.y) > MAX_Y_SPEED)
            // {
            //     node.velocity.y = glm::sign(node.velocity.y) * MAX_Y_SPEED;
            // }

            node.velocity = glm::vec3(xzSpeed.x, node.velocity.y, xzSpeed.z);
        }
    }
}
