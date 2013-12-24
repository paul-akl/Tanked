#include "MenuMode.h"


MenuMode::MenuMode(void)
{
	printf("create menu mode\n");
}
void MenuMode::StartGame(Game& iGame)
{
	changeMode(iGame, MAIN);
}
void MenuMode::ShowCredits(Game& iGame)
{
	//changeMode(iGame, CREDITS);
}
MenuMode::~MenuMode(void)
{
	printf("destroy menu mode\n");
}
