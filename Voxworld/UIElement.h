#pragma once
#include "SceneNode.h"
#include "TextureNode.h"
#include "MeshNode.h"
#include "TransformNode.h"
#include <sstream>
class UIElement :
	public SceneNode
{
public:
	UIElement();
	UIElement(std::string p_Image);
	virtual ~UIElement(void);	
	virtual void init()=0;
	virtual void update(float p_DeltaTimeS)=0;
	virtual void render(Renderer * p_Renderer)=0;
	
protected:
	GLuint m_Label;
	TextureNode * m_Data;
	
};

