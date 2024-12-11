#include <GLFW/glfw3.h>

namespace Raylib {
    #include <raylib.h>
}

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

#include "game/Init.hpp"
#include "game/MovePlayer.hpp"
#include "game/UpdateCamera.hpp"

// While we wait for real groupsets
void UpdateGroupSet(ES::Engine::Registry &registry)
{
    Game::UpdateCamera(registry);
    Game::MovePlayer(registry);
    ES::Plugin::Physics::System::VelocityIntegration(registry);
    ES::Plugin::Physics::System::DetectSoftBodyCollisions(registry);
    ES::Plugin::Physics::System::ApplySoftBodyCollisions(registry);
    ES::Plugin::Physics::System::DeleteSoftBodyCollisions(registry);
}

int main()
{
    ES::Engine::Registry registry;

    registry.RegisterSystem<ES::Engine::Scheduler::Startup>(Raylib::InitRenderer);
    registry.RegisterSystem<ES::Engine::Scheduler::Startup>(Game::Init);

    registry.RegisterSystem<ES::Engine::Scheduler::RelativeTimeUpdate>(UpdateGroupSet);

    registry.RegisterSystem(Raylib::GlobalRenderer);

    do {
        registry.RunSystems();
    }
    while (!Raylib::WindowShouldClose());

    Raylib::CloseWindow();

    return 0;
}
