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
	if(m_OrientationDeg >= 360.0f)
	{
		m_OrientationDeg-=360.0f;
	}
	else if(m_OrientationDeg < 0.0f)
	{
		m_OrientationDeg=360.0f+m_OrientationDeg;
	}
	bool turning = false;
	float deltaOrientation = m_TargetOrientation-m_OrientationDeg;
	if(abs(deltaOrientation) < 5.5f)
	{
		turning = false;
	}
	else
	{
		turning = true;
	}
	if(turning)
	{
		if (abs(deltaOrientation) > 180.0f)
			deltaOrientation += deltaOrientation > 0? -360.0f:360.0f;
			if(deltaOrientation < 0)
			{
				turnRight(p_DeltaTimeS);
			}
			else if(deltaOrientation > 0)
			{
				turnLeft(p_DeltaTimeS);
			}
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

void RobotHead::addDamagedTexture(TextureNode* p_Texture)
{
	m_DamagedDiffuseTexture = p_Texture;
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
	m_TargetOrientation = atan2f(p_Target.z,p_Target.x);
	if(m_TargetOrientation<0.0f)
		m_TargetOrientation+=PI*2.0f;
	m_TargetOrientation*=RAD_TO_DEG;
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
