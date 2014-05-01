#pragma once
#include "WallNode.h"
////////////////////////////////////////////////////////
//simple class to create and manage wall nodes        //
//uses object pool for memory management              //
//when no longer needed, deactivate wall node		  //
//There is code duplication here, coinciding with     //
//FloorFactory                                        //
// Perhaps creating a base class would help           //
////////////////////////////////////////////////////////
class WallFactory
{
public:
	WallFactory(void);
	void init();
	WallNode* getInstance();
	virtual ~WallFactory(void);
protected:
	static const int m_MaxWallSegments = 512;
	WallNode* getInstanceFromPool();
	std::vector<WallNode*> m_WallSegments;
	TextureNode	*m_WallDiffuse1,
				*m_WallNormal1,
				*m_WallEmissive1,
				*m_WallHeight1,
				*m_WallSpecular1;
	MeshNode* m_WallMesh1;
	bool m_ReusingObject;
	int m_NumObjects;
};

