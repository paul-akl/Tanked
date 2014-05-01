#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include "SceneNode.h"
class TransformNode:
	public SceneNode 
{
public:
	TransformNode(void);
	const glm::mat4 getLocalTransform(void);
	void reset(void);
	const glm::mat4 getWorldTransform(void);
	void translate(const glm::vec3 p_Translation);
	//this function needs parameters replaced by a quaternion,
	//so temporary function:
	//void rotate(const glm::quat& p_Rotation);
	void rotate(const float p_Angle,const glm::vec3& p_Axis);
	void scale(const glm::vec3& p_Scale);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeMS){;}
	virtual ~TransformNode(void);
private:
	glm::mat4 m_Matrix;
};

