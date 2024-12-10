#include "UpdateCamera.hpp"

#include "PlayerTag.hpp"

#include "SoftBodyBox.hpp"
#include "SoftBodyNode.hpp"

void Game::UpdateCamera(ES::Engine::Registry &registry)
{
    auto view = registry.GetRegistry().view<Game::Player, Objects::SoftBodyBox>();
    glm::vec3 middlePos;
    glm::vec3 gVelocity;

    for (auto entity : view)
    {
        auto &box = view.get<Objects::SoftBodyBox>(entity);
        middlePos = box.GetMiddlePoint(registry);
        gVelocity = box.GetVelocity(registry);
    }

    static float angleX = 0.0f;
    static float angleY = 0.0f;
    const float sensitivity = 0.005f;

    auto md = Raylib::GetMouseDelta();
    float mouseDeltaX = -md.x;
    float mouseDeltaY = md.y;

    angleX += mouseDeltaX * sensitivity;
    angleY += mouseDeltaY * sensitivity;

    const float maxVerticalAngle = glm::radians(89.0f);
    angleY = glm::clamp(angleY, -maxVerticalAngle, maxVerticalAngle);

    float distance = 8.0f;
    glm::vec3 offset;
    offset.x = distance * cos(angleY) * sin(angleX);
    offset.y = distance * sin(angleY);
    offset.z = distance * cos(angleY) * cos(angleX);

    auto view2 = registry.GetRegistry().view<Raylib::Camera3D>();
    for (auto entity : view2)
    {
        auto &camera = view2.get<Raylib::Camera3D>(entity);
        camera.target = {middlePos.x, middlePos.y, middlePos.z};
        middlePos += offset;
        camera.position = {middlePos.x, middlePos.y, middlePos.z};
    }
}
