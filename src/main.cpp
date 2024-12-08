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

int main()
{
    ES::Engine::Registry registry;

    registry.RegisterSystem<ES::Engine::Scheduler::Startup>(Raylib::InitRenderer);
    registry.RegisterSystem<ES::Engine::Scheduler::Startup>(Game::Init);

    // TODO: I don't know if this should be updated using relative time or not
    // for the future we should implement groupsets
    registry.RegisterSystem(Game::MovePlayer);
    registry.RegisterSystem(Game::UpdateCamera);

    registry.RegisterSystem<ES::Engine::Scheduler::RelativeTimeUpdate>(ES::Plugin::Physics::System::VelocityIntegration);
    registry.RegisterSystem<ES::Engine::Scheduler::RelativeTimeUpdate>(ES::Plugin::Physics::System::DetectSoftBodyCollisions);
    registry.RegisterSystem<ES::Engine::Scheduler::RelativeTimeUpdate>(ES::Plugin::Physics::System::ApplySoftBodyCollisions);
    registry.RegisterSystem<ES::Engine::Scheduler::RelativeTimeUpdate>(ES::Plugin::Physics::System::DeleteSoftBodyCollisions);

    registry.RegisterSystem(Raylib::GlobalRenderer);

    do {
        registry.RunSystems();
    }
    while (!Raylib::WindowShouldClose());

    Raylib::CloseWindow();

    return 0;
}
