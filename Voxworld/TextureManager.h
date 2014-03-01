#pragma once
class TextureManager
{
public:
	TextureManager(void);
	virtual ~TextureManager(void);

private:		
	static bool m_Instantiated;
	TextureManager* m_Instance;
};

bool TextureManager::m_Instantiated = false;