#include "WallFactory.h"
#include "TextureNode.h"
#include "MeshNode.h"
#include "TransformNode.h"
#include <sstream>

WallFactory::WallFactory(void)
{
	m_NumObjects=0;
}
void WallFactory::init()
{
	//sets up uniform data, such as textures and meshes
	m_WallDiffuse1 =  new TextureNode();
	m_WallDiffuse1->setTextureType(DIFFUSE);
	m_WallDiffuse1->setName("WallDiffuse0");
	m_WallDiffuse1->loadTexture("images/openWallWrap_D.png");

	m_WallNormal1 =  new TextureNode();
	m_WallNormal1->setTextureType(NORMAL);
	m_WallNormal1->setName("WallNormal0");
	m_WallNormal1->loadTexture("images/openWallWrap_N.png");

	m_WallEmissive1 =  new TextureNode();
	m_WallEmissive1->setTextureType(EMISSIVE);
	m_WallEmissive1->setName("WallEmissive0");
	m_WallEmissive1->loadTexture("images/openWallWrap_E.png");

	m_WallHeight1 =  new TextureNode();
	m_WallHeight1->setTextureType(HEIGHT);
	m_WallHeight1->setName("WallHeight0");
	m_WallHeight1->loadTexture("images/openWallWrap_H.png");

	m_WallMesh1 = new MeshNode();
	m_WallMesh1->setName("WallMesh0");
	m_WallMesh1->loadModel("models/cube.obj");
	m_ReusingObject = false;
}
WallNode* WallFactory::getInstanceFromPool()
{
	//manages object pool
	//looks for unused walls before creating new ones.
	if(!m_WallSegments.empty())
	{
		for (size_t i = 0; i < m_WallSegments.size(); i++)
		{
			//if object exists
			if(m_WallSegments[i]!=nullptr)
			{
				//if one is found, early bail
				if(!m_WallSegments[i]->isActive())
				{
					m_ReusingObject = true;
					return m_WallSegments[i];
				}
				//else if active, skip object, as it is in use.
				else
				{
					continue;
				}
			}
			//if, for some reason, we find a nullptr in the list, the object has been deleted elsewhere, so create a new one.
			else
			{
				m_ReusingObject = false;
				m_WallSegments[i] =  new WallNode();
				return m_WallSegments[i];
			}
		}
	}
	//pool is empty. just create and add object
	//so long as we have not reached maximum number of active objects
		m_ReusingObject = false;
		m_WallSegments.push_back(new WallNode());
		return m_WallSegments[m_WallSegments.size()-1];
}
WallNode* WallFactory::getInstance()
{
	WallNode* v_WallInstance = getInstanceFromPool();
	if(m_ReusingObject)
	{
		m_NumObjects++;
		v_WallInstance->activate();
	}
	else
	{
		m_NumObjects++;
		v_WallInstance->addMesh(m_WallMesh1);
		v_WallInstance->addTexture(m_WallDiffuse1);
		v_WallInstance->addTexture(m_WallNormal1);
		v_WallInstance->addTexture(m_WallEmissive1);
		v_WallInstance->addTexture(m_WallHeight1);
		v_WallInstance->setType(SCENERY);
		v_WallInstance->setBoundaryType(AAB);
		v_WallInstance->activate();
		TransformNode* v_TransInstance = new TransformNode();
		v_TransInstance->setName("WallTransform0");
		v_TransInstance->reset();
		v_WallInstance->setRecursive(false);
		v_WallInstance->setRadius(1.0f);
		v_WallInstance->addTransform(v_TransInstance);
	}
	std::stringstream ss;
	ss<<"Wall"<<m_NumObjects;
	v_WallInstance->setName(ss.str());

	return v_WallInstance;

}
WallFactory::~WallFactory(void)
{
	//cleanup
	delete m_WallMesh1;
	delete m_WallDiffuse1;
	while(!m_WallSegments.empty()) delete m_WallSegments.back(), m_WallSegments.pop_back();
}
