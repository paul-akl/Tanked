#pragma once
#include "scenenode.h"
struct LightStruct
{
	//ambient colour
	float m_Ambient[4];
	//surface scattering colour
	float m_Diffuse[4];
	//reflection colour
	float m_Specular[4];
	//m_Position[3] is 0 if directional light
	//"		   "[3] is 1 if point/spotlight
	float m_Position[4];
	//(0,0,0,0) if point light
	float m_Direction[4];
	//attenuation[0] is constant 0-1 range
	//"         "[1] is linear coefficient
	//"         "[2] is quadratic coefficient
	float m_Attenuation[3];
	//output, in Lumens
	float m_Strength;
};
class LightNode :
	public SceneNode
{
public:
	LightNode(void);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeMS){;}
	void setAmbientColour(glm::vec4 p_Colour);
	void setDiffuseColour(glm::vec4 p_Colour);
	void setSpecularColour(glm::vec4 p_Colour);
	void setPosition(glm::vec4 p_Position);
	void setStrength(float p_Strength);
	void setAttenuation(float p_Constant, float p_Linear, float p_Quadratic);
	void setDirection(glm::vec4 p_Direction);
	LightStruct getData(void){return m_Light;}
	virtual ~LightNode(void);
protected:
	LightStruct m_Light;
};

