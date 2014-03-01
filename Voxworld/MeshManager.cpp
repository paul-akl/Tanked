#include "MeshManager.h"
#include <assert.h>


MeshManager::MeshManager(void)
{
	assert(!m_Instantiated);
	m_Instantiated = true;
}


MeshManager::~MeshManager(void)
{
	delete this;
}
