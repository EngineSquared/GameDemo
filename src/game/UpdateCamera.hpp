#pragma once

#include "Registry.hpp"

namespace Raylib {
    #include "raylib.h"
}

#include <glm/glm.hpp>

namespace Game {
    void UpdateCamera(ES::Engine::Registry& registry);
}
