#pragma once
#include "gamemode.h"
class ShopMode :
	public GameMode
{
public:
	ShopMode(void);
	virtual void init(Game& iGame){;}
	virtual void update(float deltaTime){;}
	virtual bool handleEvent(Game& igame){return true;}
	virtual void draw(Renderer* p_Renderer){printf("drawing shop mode");}
	virtual ~ShopMode(void);
};

