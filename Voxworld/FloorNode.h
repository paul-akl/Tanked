#pragma once
#include "scenenode.h"
//simple class to represent a floor
class FloorNode :
	public SceneNode
{
public:
	FloorNode(void);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	void setSize(const float& p_Size){m_Size = p_Size;}
	virtual ~FloorNode(void);
private:
	float m_Size;
};

