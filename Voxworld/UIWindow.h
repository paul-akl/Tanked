#pragma once
#include "scenenode.h"

class UIWindow :
	public SceneNode
{
public:
	UIWindow(void);
	virtual ~UIWindow(void);
	virtual void init();
	virtual void update(float p_DeltaTimeS)=0;
	virtual void render(Renderer * p_Renderer)=0;

protected:
		
};

