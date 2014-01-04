#include "RobotHead.h"
#include "Renderer.h"
#include "TransformNode.h"
#include "TextureNode.h"
#include "MeshNode.h"
RobotHead::RobotHead(void)
{
	m_DamagedDiffuseTexture = nullptr;
	m_TurnSpeed = 5.0f;
	m_TargetOrientation = 0.0;
	m_OrientationDeg = 0.0;
}

void RobotHead::update(float p_DeltaTimeS)
{
	//normalize target orientation, if needed
	if(m_TargetOrientation >= 360.0f)
	{
		m_TargetOrientation-=360.0f;
	}
	else if(m_TargetOrientation < 0.0f)
	{
		m_TargetOrientation=360.0f+m_TargetOrientation;
	}
	//normalize current orientation if needed
	if(m_OrientationDeg >= 360.0f)
	{
		m_OrientationDeg-=360.0f;
	}
	else if(m_OrientationDeg < 0.0f)
	{
		m_OrientationDeg=360.0f+m_OrientationDeg;
	}
	if(m_TargetOrientation!=m_OrientationDeg)
	{
		if(abs(m_TargetOrientation-m_OrientationDeg)<5.5f)
		{
			m_OrientationDeg = m_TargetOrientation;
			m_Turning = false;
		}
		else
		{
			m_Turning = true;
			float deltaAngle = m_TargetOrientation-m_OrientationDeg;
			//if theta is greater than 180 degrees, then make a small adjustment, to make deltaAngle relative to zero
			//(make deltaAngle either positive or negative
			if (abs(deltaAngle) > 180.0f)
				deltaAngle += deltaAngle > 0? -360.0f:360.0f;
			if(deltaAngle < 0)
			{
				turnLeft(p_DeltaTimeS);
			}
			else if(deltaAngle > 0)
			{
				turnRight(p_DeltaTimeS);
			}
		}
	}
	m_LocalTransform->reset();
	m_LocalTransform->translate(m_Position);
	m_LocalTransform->rotate(m_OrientationDeg,glm::vec3(0.0f,1.0f,0.0f));
	//m_LocalTransform->scale(glm::vec3(m_Radius));
	SceneNode::update(p_DeltaTimeS);
}
void RobotHead::setDamaged(bool p_Damaged)
{
	m_Damaged = p_Damaged;
}
bool RobotHead::isDamaged()
{
	return m_Damaged;
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
void RobotHead::LookAt(const glm::vec3& p_Target)
{
	m_TargetOrientation = atan2(p_Target.x,p_Target.z)*RAD_TO_DEG;
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
