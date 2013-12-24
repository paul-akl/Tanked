#include "MaterialNode.h"
#include "GLTestRender.h"

MaterialNode::MaterialNode(void)
{
}
void MaterialNode::render(Renderer* p_Renderer)
{
	p_Renderer->render(this);
}
void MaterialNode::update(float p_DeltaTimeMS)
{
	SceneNode::update(p_DeltaTimeMS);
}
void MaterialNode::setAmbient(glm::vec4 p_Ambient)
{
	m_MaterialData.ambient[0] = p_Ambient.r;
	m_MaterialData.ambient[1] = p_Ambient.g;
	m_MaterialData.ambient[2] = p_Ambient.b;
	m_MaterialData.ambient[3] = p_Ambient.a;
}
void MaterialNode::setDiffuse(glm::vec4 p_Diffuse)
{
	m_MaterialData.diffuse[0] = p_Diffuse.r;
	m_MaterialData.diffuse[1] = p_Diffuse.g;
	m_MaterialData.diffuse[2] = p_Diffuse.b;
	m_MaterialData.diffuse[3] = p_Diffuse.a;
}
void MaterialNode::setSpecular(glm::vec4 p_Specular)
{
	m_MaterialData.specular[0] = p_Specular.r;
	m_MaterialData.specular[1] = p_Specular.g;
	m_MaterialData.specular[2] = p_Specular.b;
	m_MaterialData.specular[3] = p_Specular.a;
}
void MaterialNode::setEmissive(glm::vec4 p_Emissive)
{
	m_MaterialData.emissive[0] = p_Emissive.r;
	m_MaterialData.emissive[1] = p_Emissive.g;
	m_MaterialData.emissive[2] = p_Emissive.b;
	m_MaterialData.emissive[3] = p_Emissive.a;
}
void MaterialNode::setShininess(float p_Shininess)
{
	m_MaterialData.shininess = p_Shininess;
}
Material* MaterialNode::getMaterialData()
{
	return &m_MaterialData;
}
MaterialNode::~MaterialNode(void)
{
}
