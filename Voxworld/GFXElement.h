#pragma once
#include "uielement.h"
class GFXElement :
	public UIElement
{
public:
	GFXElement(void);
	virtual ~GFXElement(void);
	virtual void init();
	virtual void update(float p_DeltaTimeS);
	virtual void render(Renderer * p_Renderer);
};

