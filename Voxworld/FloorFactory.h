#pragma once
#include "FloorNode.h"
//simple class to create floor objects
//updated to incorporate uniform texture and meshes
//and floor references are kept in an object pool
//no outside memory management necessary
//when you no longer need the floor segment, simply deactivate it
class FloorFactory
{
public:
	FloorFactory(void);
	void init();
	FloorNode* getFloor();
	~FloorFactory(void);
protected:
	static const int m_MaxFloorSegments = 512;
	FloorNode* getFloorFromPool();
	std::vector<FloorNode*> m_FloorPool;
	TextureNode* m_FloorTexture1;
	MeshNode* m_FloorMesh1;
	bool m_Reusing;
};

