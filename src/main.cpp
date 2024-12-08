#include <GLFW/glfw3.h>

#include <raylib.h>

#include <iostream>
#include <chrono>

#include "Registry.hpp"
#include "BoxCollider3D.hpp"
#include "Camera3D.hpp"
#include "SoftBodyCollision.hpp"
#include "SoftBodyNode.hpp"
#include "SoftBodySpring.hpp"
#include "Transform.hpp"
#include "VelocityIntegration.hpp"
#include "Physics.hpp"

#include "RaylibRenderer.hpp"

#include "SoftBodyBox.hpp"

int main()
{
    ES::Engine::Registry registry;

    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "esq game");
    DisableCursor();
    SetTargetFPS(60);

    Camera3D raylibCam = {
        {10, 10, 10},
        {0,  0,  0 },
        {0,  1,  0 },
        45.0f,
        CAMERA_PERSPECTIVE
    };

    registry.RegisterSystem(ES::Plugin::Physics::System::DeleteSoftBodyCollisions);
    registry.RegisterSystem(ES::Plugin::Physics::System::VelocityIntegration);
    registry.RegisterSystem(ES::Plugin::Physics::System::DetectSoftBodyCollisions);
    registry.RegisterSystem(ES::Plugin::Physics::System::ApplySoftBodyCollisions);
    registry.RegisterSystem(Raylib::GlobalRenderer);

    auto softBodyBox = Objects::CreateSoftBodyBox(registry, glm::vec3(-2, 7, 2), glm::vec3(1, 1, 1));

    ES::Engine::Entity ground = registry.CreateEntity();
    registry.GetRegistry().emplace<ES::Plugin::Object::Component::Transform>(ground, glm::vec3(0, 0, 0));
    registry.GetRegistry().emplace<ES::Plugin::Physics::Component::BoxCollider3D>(ground, glm::vec3(20, 1, 20));

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        UpdateCamera(&raylibCam, CAMERA_FREE);

        BeginMode3D(raylibCam);

        registry.RunSystems();

        EndMode3D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
