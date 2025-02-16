#include <GLFW/glfw3.h>

namespace Raylib {
    #include <raylib.h>
}

#include <iostream>
#include <chrono>

#include "Core.hpp"
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
void UpdateGroupSet(ES::Engine::Core &core)
{
    Game::UpdateCamera(core);
    Game::MovePlayer(core);
    ES::Plugin::Physics::System::VelocityIntegration(core);
    ES::Plugin::Physics::System::DetectSoftBodyCollisions(core);
    ES::Plugin::Physics::System::ApplySoftBodyCollisions(core);
    ES::Plugin::Physics::System::DeleteSoftBodyCollisions(core);
}

int main()
{
    ES::Engine::Core core;

    core.RegisterSystem<ES::Engine::Scheduler::Startup>(Raylib::InitRenderer);
    core.RegisterSystem<ES::Engine::Scheduler::Startup>(Game::Init);

    core.RegisterSystem<ES::Engine::Scheduler::RelativeTimeUpdate>(UpdateGroupSet);

    core.RegisterSystem(Raylib::GlobalRenderer);

    do {
        core.RunSystems();
    }
    while (!Raylib::WindowShouldClose());

    Raylib::CloseWindow();

    return 0;
}
