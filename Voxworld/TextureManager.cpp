#include "TextureManager.h"
#include <assert.h>


TextureManager::TextureManager(void)
{
	assert(!m_Instantiated);
	m_Instantiated = true;
}


TextureManager::~TextureManager(void)
{
}
