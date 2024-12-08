#include "UpdateCamera.hpp"

#include "PlayerTag.hpp"

#include "SoftBodyBox.hpp"
#include "SoftBodyNode.hpp"

void Game::UpdateCamera(ES::Engine::Registry &registry)
{
    auto view = registry.GetRegistry().view<Game::PlayerTag, Objects::SoftBodyBox>();
    glm::vec3 middlePos;
    glm::vec3 gVelocity;

    for (auto entity : view)
    {
        auto &box = view.get<Objects::SoftBodyBox>(entity);
        middlePos = box.GetMiddlePoint(registry);
        gVelocity = box.GetVelocity(registry);
    }

    auto view2 = registry.GetRegistry().view<Raylib::Camera3D>();

    if (glm::length(gVelocity) < 1) {
        return;
    }

    gVelocity[1] = 0;
    gVelocity = glm::normalize(gVelocity);

    for (auto entity : view2)
    {
        auto &camera = view2.get<Raylib::Camera3D>(entity);
        camera.position = {middlePos.x, middlePos.y + 10, middlePos.z + 10};
        camera.target = {middlePos.x, middlePos.y, middlePos.z};
    }
}