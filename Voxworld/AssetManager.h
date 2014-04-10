#pragma once
#include "AudioSystem.h"
//#include "TextureManager.h"
//#include "MeshManager.h"

class AssetManager
{
public:
	AssetManager(void);
	~AssetManager(void);
	AudioSystem* getAudioSystem(){return m_AudioSystem;}
	//TextureManager* getTextureManager(){return m_TextureManager;}
	//MeshManager* getMeshManager(){return m_MeshManager;}
	void updateSubSystems();
private:
	AudioSystem* m_AudioSystem;
	//TextureManager* m_TextureManager;
	//MeshManager* m_MeshManager;

};

