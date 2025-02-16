#pragma once

#include "Core.hpp"

// TODO: replace this with the vk renderer when available

namespace Raylib {
    #include "raylib.h"

    void SoftBodySpringRenderer(ES::Engine::Core &core);
    void BoxColliderRenderer(ES::Engine::Core &core);
    void FakeMeshRenderer(ES::Engine::Core &core);

    Camera3D FindRaylibCamera(ES::Engine::Core &core);

    void InitRenderer(ES::Engine::Core &core);
    void GlobalRenderer(ES::Engine::Core &core);
}