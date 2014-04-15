#pragma once
#include "scenenode.h"
struct Attenuation
{
	float m_Constant;
	float m_Linear;
	float m_Quadratic;
	Attenuation(): m_Constant(0.0f),m_Linear(0.0f), m_Quadratic(0.0f){;}
};
struct LightStruct
{
	glm::mat4 m_LightModel;
	glm::vec3 m_Colour;
	float m_AmbientI, m_DiffuseI, m_SpecI, m_SpecP;
	Attenuation m_Attenuation;
	LightStruct(): m_AmbientI(0.0f), m_DiffuseI(0.0f){;}
};

class LightNode :
	public SceneNode
{
public:
	LightNode(void);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeMS);
	LightStruct getBaseData(){return m_BaseData;}

	float getAttenConstant()	{ return m_BaseData.m_Attenuation.m_Constant;	}
	float getAttenLinear()		{ return m_BaseData.m_Attenuation.m_Linear;		}
	float getAttenQuadratic()	{ return m_BaseData.m_Attenuation.m_Quadratic;	}
	float getAmbientIntensity()	{ return m_BaseData.m_AmbientI;		}
	float getDiffuseIntensity()	{ return m_BaseData.m_DiffuseI;		}
	float getSpecularIntensity(){ return m_BaseData.m_SpecI;		}
	float getSpecularPower()	{ return m_BaseData.m_SpecP;		}
	glm::mat4 getLightModel()	{ return m_BaseData.m_LightModel;	}
	glm::vec3 getColour()		{ return m_BaseData.m_Colour;		}
	glm::vec3 getAttenuation()	{ return glm::vec3(	m_BaseData.m_Attenuation.m_Constant,
													m_BaseData.m_Attenuation.m_Linear,
													m_BaseData.m_Attenuation.m_Quadratic ); }

	void setAttenuation(float p_Constant, float p_Linear, float p_Quadratic);
	void setColour(glm::vec3 p_Colour){ m_BaseData.m_Colour = p_Colour; }
	void setAmbientIntensity(float p_AmbientIntensity){ m_BaseData.m_AmbientI = p_AmbientIntensity;}
	void setDiffuseIntensity(float p_DiffuseIntensity){ m_BaseData.m_DiffuseI = p_DiffuseIntensity;}
	void setSpecularIntensity(float p_SpecularI){m_BaseData.m_SpecI = p_SpecularI;}
	void setSpecularPower(float p_SpecularP){m_BaseData.m_SpecP = p_SpecularP;}

	virtual ~LightNode(void);
protected:
	LightStruct m_BaseData;
};

