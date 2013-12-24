#pragma once
#include "CollidableNode.h"
////////////////////////////////////////////////////////////////////
//standard class to represent a wall in the world we build        //
//Very simple class, mostly using default SceneNode behaviours    //
//But, can be extended later to include Sections with lights      //
////////////////////////////////////////////////////////////////////
class WallNode :
	public CollidableNode
{
public:
	WallNode(void);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	virtual ~WallNode(void);
};

