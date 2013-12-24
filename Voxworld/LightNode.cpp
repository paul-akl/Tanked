#include "LightNode.h"
#include "GLTestRender.h"

LightNode::LightNode(void)
{
}
void LightNode::render(Renderer* p_Renderer)
{
	p_Renderer->render(this);
}
void LightNode::setAmbientColour(glm::vec4 p_Colour)
{
	m_Light.m_Ambient[0]=p_Colour.x;
	m_Light.m_Ambient[1]=p_Colour.y;
	m_Light.m_Ambient[2]=p_Colour.z;
	m_Light.m_Ambient[3]=p_Colour.w;
}
void LightNode::setDiffuseColour(glm::vec4 p_Colour)
{
	m_Light.m_Diffuse[0]=p_Colour.x;
	m_Light.m_Diffuse[1]=p_Colour.y;
	m_Light.m_Diffuse[2]=p_Colour.z;
	m_Light.m_Diffuse[3]=p_Colour.w;
}
void LightNode::setSpecularColour(glm::vec4 p_Colour)
{
	m_Light.m_Specular[0]=p_Colour.x;
	m_Light.m_Specular[1]=p_Colour.y;
	m_Light.m_Specular[2]=p_Colour.z;
	m_Light.m_Specular[3]=p_Colour.w;
}
void LightNode::setDirection(glm::vec4 p_Direction)
{
	m_Light.m_Direction[0] = p_Direction.x;
	m_Light.m_Direction[1] = p_Direction.y;
	m_Light.m_Direction[2] = p_Direction.z;
	m_Light.m_Direction[3] = p_Direction.w;
}
void LightNode::setPosition(glm::vec4 p_Position)
{
	m_Light.m_Position[0] = p_Position.x;
	m_Light.m_Position[1] = p_Position.y;
	m_Light.m_Position[2] = p_Position.z;
	m_Light.m_Position[3] = p_Position.w;
}
void LightNode::setStrength(float p_Strength)
{
	m_Light.m_Strength = p_Strength;
}
void LightNode::setAttenuation(float p_Constant, float p_Linear, float p_Quadratic)
{
	m_Light.m_Attenuation[0] = p_Constant;
	m_Light.m_Attenuation[1] = p_Linear;
	m_Light.m_Attenuation[2] = p_Quadratic;
}
LightNode::~LightNode(void)
{
}
