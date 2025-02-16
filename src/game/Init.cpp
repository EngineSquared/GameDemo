#include "Init.hpp"

void Game::Init(ES::Engine::Core& core)
{
    InitPlayer(core);
    InitTerrain(core);
}
