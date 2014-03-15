#include "Plane.h"


Plane::Plane(void) : m_Normal(0,0,0), m_Distance(NULL)
{
}

void Plane::updateValues(const glm::vec3& p_Normal, float p_Distance, bool p_Normalize)
{
	if(p_Normalize)
	{
		float length = sqrt(glm::dot(p_Normal,p_Normal));

		m_Normal = p_Normal/length;
		m_Distance = p_Distance/length;
	}
	else
	{
		m_Normal = p_Normal;
		m_Distance = p_Distance;
	}
}

Plane::~Plane(void)
{
}
