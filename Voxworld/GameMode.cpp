#include "GameMode.h"
#include "Game.h"
#include <stdio.h>
GameMode::GameMode(void)
{
}

void GameMode::changeMode(Game& game, GameState state)
{
	//this is needed for changing states, as friendship is not inherited to concrete GameModes, so changing of gamestates must be delegated.
	game.changeState(state);
}

GameMode::~GameMode(void)
{

}
