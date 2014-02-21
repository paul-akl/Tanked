#pragma once
#include "scenenode.h"
#include <SDL_ttf.h>
enum TextureType
{
	DIFFUSE=0,
	SPECULAR,
	NORMAL,
	DEPTH,
};
class Renderer;

class TextureNode :
	public SceneNode
{
public:
	TextureNode(void);
	//throws exception
	bool loadTexture(const std::string& p_FileName);
	GLuint getTexture(void){return m_Texture;}
	void setTexture(const GLuint p_Texture){m_Texture = p_Texture;}
	TextureType getTextureType(void){return m_Type;}
	void setTextureType(TextureType p_Type){m_Type = p_Type;}
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeMS){;}
	virtual ~TextureNode(void);
private:
	GLuint m_Texture;
	TextureType m_Type;
	
};

