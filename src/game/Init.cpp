#include "Init.hpp"

void Game::Init(ES::Engine::Registry& registry)
{
    InitPlayer(registry);
    InitTerrain(registry);
}
