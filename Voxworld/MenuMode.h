#pragma once
#include "gamemode.h"

//////////////////////////////////////////////////////////////////
// Class which handles menu related stuff, like game settings	//
// control mapping, and basic functions such as exiting the game//
// creating a new game, loading a previous one                  //
// From this class, users will be able to navigate to other		//
// Game Modes, such as credits, replaying intro, and of course  //
// The main Game mode.											//
//////////////////////////////////////////////////////////////////
class MenuMode :
	public GameMode
{
public:
	MenuMode(void);
	virtual void init(Game& iGame,Renderer* P_Renderer){printf("init menu mode\n");}
	virtual void update(float deltaTime){printf("update menu mode\n");}
	virtual bool handleEvent(Game& igame){return true;}
	virtual void draw(Renderer* p_Renderer){printf("drawing menu mode\n");}
	~MenuMode(void);
private:
	//switches current Game mode to MainGameMode
	void StartGame(Game& iGame);
	//credits mode not yet implemented
	void ShowCredits(Game& iGame);
};

