#include "FloorFactory.h"
#include "TextureNode.h"
#include "MeshNode.h"
#include "TransformNode.h"
#include <sstream>

FloorFactory::FloorFactory(void)
{
}
FloorNode* FloorFactory::getFloorFromPool()
{
	//this function manages the object pool
	//minimizes the number of objects required to be created at any time
	size_t index = 0;
	//if list is empty then it's fine to just create one and slot it in to the pool
	if(m_FloorPool.empty())
	{
		m_FloorPool.push_back(new FloorNode());
		m_Reusing = false;
		return m_FloorPool[0];
	}
	//if not empty then we need to check objects to find a slot that isn't in use
	else
	{
		for (size_t i = 0; i < m_FloorPool.size();i++)
		{
			//if object exists
			if(m_FloorPool[i] != nullptr)
			{
				//if object has been deactivated, early loop exit. return current object
				if(!m_FloorPool[i]->isActive())
				{
					m_Reusing = true;
					return m_FloorPool[i];
				}
				//else we have an object which is in use, so skip it
				else
				{
					continue;
				}
			}
		}
	}
	//if we reach this stage, then a suitable slot has not been found
	//so if we are not at maximum, then we can simply add a new one,
	//and return the last element in the list
	m_FloorPool.push_back(new FloorNode());
	m_Reusing = false;
	return m_FloorPool.back();
}
void FloorFactory::init()
{
	//set up our non unique data, such as uniform textures and meshes
	m_FloorMesh1 = new MeshNode();
	m_FloorTexture1 = new TextureNode();
	m_FloorTexture1->setTextureType(DIFFUSE);
	m_FloorTexture1->loadTexture("images/studdedmetal.png");
	m_FloorTexture1->setName("floor0diffuse0");
	m_FloorMesh1->setName("floor0mesh0");
	m_FloorMesh1->loadModel("plane.obj");
}
FloorNode* FloorFactory::getFloor()
{


		//instantiate a floor node from the object pool
		FloorNode* floorInstance  = getFloorFromPool();
		if (!m_Reusing)
		{
			std::stringstream ss;
			ss<<"floor"<<m_FloorPool.size();
			floorInstance->setName(ss.str().c_str());
			//set to two metres off the ground
			TransformNode* floorTransform = new TransformNode();
			floorTransform->reset();
			//set it's name
			ss<<"transform0";
			floorTransform->setName(ss.str().c_str());
			//this might be better to add to the mesh instead of the tank itself.
			//This way, multiple meshes can be added to a tank.
			floorInstance->addTexture(m_FloorTexture1);
			floorInstance->addMesh(m_FloorMesh1);
			floorInstance->addTransform(floorTransform);
		}

		return floorInstance;
}
FloorFactory::~FloorFactory(void)
{
	m_FloorPool.clear();
	delete m_FloorTexture1;
	delete m_FloorMesh1;
}
