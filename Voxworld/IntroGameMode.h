#pragma once
#include "gamemode.h"

/////////////////////////////////////////////////////////
// This class is the first mode that will be activated //
// This class will only display a basic introduction   //
// Displaying logo's and such like					   //
/////////////////////////////////////////////////////////
class IntroGameMode :
	public GameMode
{
public:
	IntroGameMode(void);
	virtual void init(Game& iGame,Renderer* p_Renderer);
	virtual void update(float deltaTime){;}
	virtual bool handleEvent(Game& igame){return true;}
	virtual void draw(Renderer* p_Renderer){printf("drawing intro mode \n");}
	~IntroGameMode(void);
private:
	void StartMenu(Game& iGame);
};

