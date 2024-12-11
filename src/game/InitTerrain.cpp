#include <random>

#include "InitTerrain.hpp"

#include "SoftBodyBox.hpp"
#include "Transform.hpp"
#include "BoxCollider3D.hpp"

#include "Obstacle.hpp"

static float RandomFloat(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

static int RandomInt(int min, int max, int step = 1)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen) / step * step;
}

void Game::InitTerrain(ES::Engine::Registry& registry)
{
    ES::Engine::Entity ground = registry.CreateEntity();

    registry.GetRegistry().emplace<ES::Plugin::Object::Component::Transform>(ground, glm::vec3(0, 0, 0));
    registry.GetRegistry().emplace<ES::Plugin::Physics::Component::BoxCollider3D>(ground, glm::vec3(40, 1, 40));

    for (int i = 0; i < 15; i++)
    {
        int yPos = 3 + i * 2;

        for (int j = 0; j < 6; j++)
        {
            ES::Engine::Entity terrainBox = registry.CreateEntity();

            registry.GetRegistry().emplace<Game::Obstacle>(terrainBox);

            glm::vec3 position = glm::vec3(RandomInt(-20, 20, 2), yPos, RandomInt(-20, 20, 2));
            glm::vec3 size = glm::vec3(RandomFloat(2.5f, 4.f), 1, RandomFloat(2.5f, 4.f));

            registry.GetRegistry().emplace<ES::Plugin::Object::Component::Transform>(terrainBox, position);
            registry.GetRegistry().emplace<ES::Plugin::Physics::Component::BoxCollider3D>(terrainBox, size);
        }
    }
}
