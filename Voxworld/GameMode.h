#pragma once

#include "Renderer.h"
#include "Controller.h"
enum GameState
{
	INTRO,
	MAIN,
	MENU,
	CREDITS
};

class Game;

class GameMode
{
public:
	GameMode(void);
	virtual void init(Game& iGame)=0;
	virtual void update(float deltaTime)=0;
	virtual bool handleEvent(Game& iGame)=0;
	virtual void draw(Renderer* p_Renderer)=0;
	void setController(Controller* p_Controller){m_Controller = p_Controller;}
	virtual ~GameMode(void);
protected:
	void changeMode(Game& game,GameState state);
	Controller* m_Controller;
};

