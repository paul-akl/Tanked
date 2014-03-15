#pragma once
#include <glm\glm.hpp>
class Plane
{
public:
	Plane(void);
	virtual ~Plane(void);

	void setNormal(const glm::vec3& p_Normal){m_Normal = p_Normal;}
	glm::vec3 getNormal() const {return m_Normal;}
	void setDistance(float p_Distance){m_Distance = p_Distance;}
	float getDistance() const {return m_Distance;}
	void updateValues(const glm::vec3& p_Normal, float p_Distance, bool p_Normalize);
	
protected:
	glm::vec3 m_Normal;
	float m_Distance;
	
};

