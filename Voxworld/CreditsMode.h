#pragma once
#include "gamemode.h"
class CreditsMode :
	public GameMode
{
public:
	CreditsMode(void);
	virtual void init(Game& iGame,Renderer* p_Renderer){printf("init credits\n");}
	virtual void update(float deltaTime){printf("update credits\n");}
	virtual bool handleEvent(Game& iGame){return true;}
	virtual void draw(Renderer* p_Renderer){printf("draw credits mode");}
	~CreditsMode(void);
private:
	void OpenMenu(Game& iGame);
};

