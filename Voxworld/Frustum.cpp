#include "Frustum.h"
#include<glm\glm.hpp>

Frustum::Frustum(void)
{
	for( int i = 0; i < MAX_PLANES; i++)
		m_Planes[i] = new Plane();
}

Frustum::~Frustum(void)
{
	for( int i = 0; i < MAX_PLANES; i++)
		delete m_Planes[i];
	//delete[] m_Planes;
}

void Frustum::deriveFrustum( glm::mat4& p_ViewMatrix,  glm::mat4& p_ProjMatrix)
{
	glm::mat4 viewProj(p_ProjMatrix * p_ViewMatrix);
	glm::vec3 x(viewProj[0].x, viewProj[1].x, viewProj[2].x);
	glm::vec3 y(viewProj[0].y, viewProj[1].y, viewProj[2].y);
	glm::vec3 z(viewProj[0].z, viewProj[1].z, viewProj[2].z);
	glm::vec3 w(viewProj[0].w, viewProj[1].w, viewProj[2].w);
	
	//right plane
	m_Planes[0]->updateValues(w - x, (viewProj[3].w - viewProj[3].x), true);
	//left plane
	m_Planes[1]->updateValues(w + x, (viewProj[3].w + viewProj[3].x), true);
	//bottom plane
	m_Planes[2]->updateValues(w + y, (viewProj[3].w + viewProj[3].y), true);
	//top plane
	m_Planes[3]->updateValues(w - y, (viewProj[3].w - viewProj[3].y), true);
	//far plane
	m_Planes[4]->updateValues(w - z, (viewProj[3].w - viewProj[3].z), true);
	//near plane
	m_Planes[5]->updateValues(w + z, (viewProj[3].w + viewProj[3].z), true);

	
}

bool Frustum::frustumCheck(glm::vec3 p_WorldPos, float p_BoundingRadius)
{
	float distance = 0;
	for(int i = 0; i < MAX_PLANES; i++)
	{
		//distance = m_Planes[i]->getDistance()+glm::dot(m_Planes[i]->getNormal(),p_BoundingRadius);
		distance = m_Planes[i]->getNormal().x*p_WorldPos.x + m_Planes[i]->getNormal().y*p_WorldPos.y + m_Planes[i]->getNormal().z*p_WorldPos.z + m_Planes[i]->getDistance();
		if( distance <= -p_BoundingRadius )
		{
			return false;
		}
		
	}
	return true;
}