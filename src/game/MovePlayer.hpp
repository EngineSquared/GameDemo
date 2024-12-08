#pragma once

#include "Registry.hpp"

namespace Raylib {
    #include "raylib.h"
}

#include <glm/glm.hpp>

namespace Game {
    void MovePlayer(ES::Engine::Registry& registry);
}
