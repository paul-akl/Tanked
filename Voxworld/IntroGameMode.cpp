#include "IntroGameMode.h"


IntroGameMode::IntroGameMode(void)
{
	printf("creating intro mode \n");
}
void IntroGameMode::init(Game& iGame)
{
	printf("init intro mode \n");
}
void IntroGameMode::StartMenu(Game& iGame)
{
	changeMode(iGame,MENU);
}
IntroGameMode::~IntroGameMode(void)
{
	printf("destroying intro mode \n");
}
