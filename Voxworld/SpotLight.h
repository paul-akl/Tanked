#pragma once
#include "lightnode.h"
struct TempWorldData
{
	glm::vec3 m_TransformedPosition;
	glm::vec3 m_TransformedDirection;
};
class SpotLight :
	public LightNode
{
public:
	SpotLight(void);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	void setTransform(glm::mat4& p_WorldTransform);
	glm::vec3 getDirection(){return m_Direction;}
	void setDirection(glm::vec3 p_Direction){m_Direction = p_Direction;}
	//void setDirection(float p_YAngleDeg);//overload not yet needed
	void setCutoffAngle(float p_CutOffDeg){m_CutoffAngleDeg =p_CutOffDeg ;}
	float getCutoffAngle(){return m_CutoffAngleDeg;}
	glm::vec3 getTransformedPosition(){return m_TempData.m_TransformedPosition;}
	glm::vec3 getTransformedDirection(){return m_TempData.m_TransformedDirection;}
	virtual ~SpotLight(void);
protected:
	glm::vec3 m_Direction;
	float m_CutoffAngleDeg;
	TempWorldData m_TempData;
};

