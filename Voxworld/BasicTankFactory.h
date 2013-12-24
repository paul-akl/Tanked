#pragma once
#include "TestTankNode.h"
// a simple factory class to quickly and automatically assemble a tank object for testing
// limited to one instance at the moment. is is assumed that all tanks in the game are unique
// 
class BasicTankFactory
{
public:
	BasicTankFactory(void);
	void init();
	TestTankNode* getTank(void);
	~BasicTankFactory(void);
private:
	static const int m_MaxNumInstances = 1;
	int m_NumInstances;
	TestTankNode* m_Instance;
	TextureNode* m_BodyDiffuse1;
	TextureNode* m_TurretDiffuse1;
	MeshNode* m_BodyMesh1;
	MeshNode* m_TurretMesh1;

};

