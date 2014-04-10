#pragma once
#include "I_Audio.h"
class I_MeshManager;
class I_TextureManager;


class Locator
{
public:
	static I_Audio* getAudioSystem(){return m_AudioService;}
	static I_MeshManager* getMeshManager(){return m_MeshService;}
	static I_TextureManager* getTextureManager(){return m_TextureService;}
	static void provide(I_Audio* p_Service){m_AudioService = p_Service;}
	static void provide(I_MeshManager* p_Service){m_MeshService = p_Service;}
	static void provide(I_TextureManager* p_Service){m_TextureService = p_Service;}

private:
	static I_Audio* m_AudioService;
	static I_MeshManager* m_MeshService;
	static I_TextureManager* m_TextureService;
};

