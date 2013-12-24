#pragma once
#include "scenenode.h"

//////////////////////////////////////////////////////
//Class to store material data for rendering		//
//////////////////////////////////////////////////////
struct Material
{
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emissive[4];
	float shininess;
};

class MaterialNode :
	public SceneNode
{
public:
	MaterialNode(void);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeMS);
	void setAmbient(glm::vec4 p_Ambient);
	void setDiffuse(glm::vec4 p_Diffuse);
	void setSpecular(glm::vec4 p_Specular);
	void setEmissive(glm::vec4 p_Emissive);
	void setShininess(float p_Shininess);
	Material* getMaterialData();
	virtual ~MaterialNode(void);
private:
	Material m_MaterialData;
};

