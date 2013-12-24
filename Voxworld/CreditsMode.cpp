#include "CreditsMode.h"


CreditsMode::CreditsMode(void)
{
}

void CreditsMode::OpenMenu(Game& iGame)
{
	changeMode(iGame, MENU);
}
CreditsMode::~CreditsMode(void)
{
	printf("destroy credits");
}
