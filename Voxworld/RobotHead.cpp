#include "RobotHead.h"
#include "Renderer.h"
#include "TransformNode.h"
#include "TextureNode.h"
#include "MeshNode.h"
RobotHead::RobotHead(void)
{
	m_DamagedTextureDiffuse = nullptr;
	m_DamagedTextureEmissive = nullptr;
	m_DamagedTextureNormal = nullptr;
	m_DamagedTextureSpecular = nullptr;
	m_TurnSpeed = 60.0f;
	m_TargetOrientation = 0.0f;
	m_OrientationDeg = 0.0f;
	m_Damaged = false;
}

void RobotHead::update(float p_DeltaTimeS)
{

	float orientationRad = m_OrientationDeg*PI_OVER180;
	float targetOrientationRad = m_TargetOrientation*PI_OVER180;
	//create a heading vector from current orientation
	glm::vec3 heading(sin(orientationRad),0.0f,cos(orientationRad));
	//and a vector to target
	glm::vec3 toTarget = glm::normalize(m_LookTarget-m_Parent->getLocation());

	//to tell if the target needs turned let or right towards, we need a reference direction
	//this can be any arbitrary non changing vector
	glm::vec3 reference(0.0f,0.0f,-1.0f);
	//lets get the relationship of each vector to the reference
	float h = glm::dot(heading,reference);
	float t = glm::dot(toTarget,reference);
	//and now to each other, based on those relationships
	float ht = t-h;
	//if the difference is noticable
	//(this stops jittering)
	if(abs(ht) > 0.01f)
	{
		//turn right
		if(ht < 0.0f)
		{
			turnRight(p_DeltaTimeS);
		}
		//turn left
		else
		{
			turnLeft(p_DeltaTimeS);
		}
		//now calculate rotation for head
	}


	m_LocalTransform->reset();
	m_LocalTransform->rotate(-m_Parent->getOrientation(),glm::vec3(0.0,1.0,0.0));
	m_LocalTransform->translate(m_Position);
	m_LocalTransform->rotate(m_OrientationDeg,glm::vec3(0.0f,1.0f,0.0f));
	m_LocalTransform->scale(glm::vec3(1.0f));
	//m_LocalTransform->scale(glm::vec3(m_Radius));
	SceneNode::update(p_DeltaTimeS);

	//m_WorldTransform = m_LocalTransform->getLocalTransform();
}

void RobotHead::turnLeft(float p_DeltaTimeS)
{
	m_OrientationDeg+=m_TurnSpeed*p_DeltaTimeS;
}
void RobotHead::turnRight(float p_DeltaTimeS)
{
	m_OrientationDeg-=m_TurnSpeed*p_DeltaTimeS;
}
void RobotHead::LookAt(const glm::vec3 p_Target)
{
	m_LookTarget = p_Target;
}
void RobotHead::rotate(float p_Rotation)
{
	m_TargetOrientation += p_Rotation;
}
void RobotHead::render(Renderer* p_Renderer)
{
	if(m_Damaged)
	{
		if(m_Mesh!=nullptr && m_DamagedTextureDiffuse!=nullptr && m_LocalTransform != nullptr)
		{
			p_Renderer->begin();
			m_DamagedTextureDiffuse->render(p_Renderer);
			if(m_DamagedTextureEmissive != nullptr)
				m_DamagedTextureEmissive->render(p_Renderer);
			if(m_DamagedTextureNormal != nullptr)
				m_DamagedTextureNormal->render(p_Renderer);
			if(m_DamagedTextureSpecular != nullptr)
				m_DamagedTextureSpecular->render(p_Renderer);
			m_LocalTransform->render(p_Renderer);
			//any other texture render calls go in here
			m_Mesh->render(p_Renderer);
			p_Renderer->end();
		}
	}
	else
	{
		SceneNode::render(p_Renderer);
	}
}
RobotHead::~RobotHead(void)
{
	m_DamagedTextureDiffuse = nullptr;
	m_DamagedTextureEmissive = nullptr;
	m_DamagedTextureNormal = nullptr;
	m_DamagedTextureSpecular = nullptr;
}
