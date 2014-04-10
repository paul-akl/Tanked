#include "RobotHead.h"
#include "Renderer.h"
#include "TransformNode.h"
#include "TextureNode.h"
#include "MeshNode.h"
RobotHead::RobotHead(void)
{
	m_DamagedDiffuseTexture = nullptr;
	m_TurnSpeed = 30.0f;
	m_TargetOrientation = 0.0f;
	m_OrientationDeg = 0.0f;
	m_Damaged = false;
}

void RobotHead::update(float p_DeltaTimeS)
{


	m_LocalTransform->reset();
	m_LocalTransform->rotate(-m_Parent->getOrientation(),glm::vec3(0.0,1.0,0.0));
	m_LocalTransform->translate(m_Position);
	m_LocalTransform->rotate(m_OrientationDeg,glm::vec3(0.0f,1.0f,0.0f));
	m_LocalTransform->scale(glm::vec3(1.0f));
	//m_LocalTransform->scale(glm::vec3(m_Radius));
	SceneNode::update(p_DeltaTimeS);

	//m_WorldTransform = m_LocalTransform->getLocalTransform();
}

void RobotHead::addDamagedTexture(TextureNode* p_Texture)
{
	m_DamagedDiffuseTexture = p_Texture;
}
void RobotHead::turnLeft(float p_DeltaTimeS)
{
	m_OrientationDeg-=m_TurnSpeed*p_DeltaTimeS;
}
void RobotHead::turnRight(float p_DeltaTimeS)
{
	m_OrientationDeg+=m_TurnSpeed*p_DeltaTimeS;
}
void RobotHead::LookAt(const glm::vec3 p_Target)
{
	m_OrientationDeg = (atan2(p_Target.z,p_Target.x));
}
void RobotHead::rotate(float p_Rotation)
{
	m_TargetOrientation += p_Rotation;
}
void RobotHead::render(Renderer* p_Renderer)
{
	if(m_Damaged)
	{
		if(m_Mesh!=nullptr && m_DamagedDiffuseTexture!=nullptr && m_LocalTransform != nullptr)
		{
			p_Renderer->begin();
			m_DamagedDiffuseTexture->render(p_Renderer);
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
	m_DamagedDiffuseTexture = nullptr;
}
