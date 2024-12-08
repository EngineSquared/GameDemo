#pragma once

#include "Registry.hpp"

// TODO: replace this with the vk renderer when available

namespace Raylib {
    #include "raylib.h"

    void SoftBodySpringRenderer(ES::Engine::Registry &registry);
    void BoxColliderRenderer(ES::Engine::Registry &registry);
    void FakeMeshRenderer(ES::Engine::Registry &registry);

    Camera3D FindRaylibCamera(ES::Engine::Registry &registry);

    void InitRenderer(ES::Engine::Registry &registry);
    void GlobalRenderer(ES::Engine::Registry &registry);
}