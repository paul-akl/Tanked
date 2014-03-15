#pragma once
#include <glm\glm.hpp>
#include "Plane.h"

////////////////////////////////////////////////////////////////////////////
// This class constructs view frustum planes by extracting elements
// from the current view-projection matrix.
// These planes can then be used to test whether an object is within
// the view frustum by comparing the distance between the plane and a point
// and detecting which side of the plane it lies on
////////////////////////////////////////////////////////////////////////////

class Frustum
{
public:
	Frustum(void);
	virtual ~Frustum(void);
	void deriveFrustum( glm::mat4& p_ViewMatrix,  glm::mat4& p_ProjMatrix);
	//check render radius intersection against planes
	bool frustumCheck(glm::vec3 p_WorldPos, float p_BoundingRadius);
	
private:
	enum {MAX_PLANES = 6};
	Plane* m_Planes[MAX_PLANES];
		
};

