#pragma once

#include "Engine.hpp"

#include <glm/glm.hpp>

namespace Objects
{
    // To be replaced with real mesh from the OBJ loader
    struct SoftBodyBoxMesh {
        std::array<ES::Engine::Entity, 3> entities;
    };

    struct SoftBodyBox
    {
        std::vector<ES::Engine::Entity> entities;

        void RotateFromPitchYawRoll(ES::Engine::Registry &registry, float pitch, float yaw, float roll);
    };

    ES::Engine::Entity CreateSoftBodyBox(ES::Engine::Registry &registry, const glm::vec3 &position, const glm::vec3 &size);
} // namespace Objects