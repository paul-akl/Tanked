#include "RobotArm.h"
#include "Renderer.h"
#include "TransformNode.h"
#include "TextureNode.h"
#include "MeshNode.h"
RobotArm::RobotArm(void)
{
	m_DamagedDiffuseTexture = nullptr;
	m_Raised = false;
	m_ArmRaiseSpeed = 1.0f;
	m_OrientationDeg = 0.0f;
	m_Damaged = false;
}
void RobotArm::setLeft()
{
	m_Left = true;
}
void RobotArm::raiseArm()
{
	m_Raised = true;
}
void RobotArm::lowerArm()
{
	m_Raised = false;
}
const bool& RobotArm::armRaised()
{
	return m_Raised;
}
void RobotArm::render(Renderer* p_Renderer)
{
	//draw this object with damage texture, or normal texture
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
void RobotArm::update(float p_DeltaTimeS)
{
	//set orientation of arm on Y axis, depending on state.
	if(m_Raised)
	{
		if(m_OrientationDeg < 90.0f)
		{
			m_OrientationDeg+=m_ArmRaiseSpeed*p_DeltaTimeS;
		}
	}
	else
	{
		if(m_OrientationDeg > 0.0f)
		{
			m_OrientationDeg+=m_ArmRaiseSpeed*p_DeltaTimeS;
		}
	}
	m_LocalTransform->reset();
	m_LocalTransform->translate(m_Position);
	m_LocalTransform->rotate(m_OrientationDeg,glm::vec3(1.0f,0.0f,0.0f));
	m_LocalTransform->translate(glm::vec3(0.0f,-1.75f,0.0f));
	SceneNode::update(p_DeltaTimeS);
}
RobotArm::~RobotArm(void)
{
	m_DamagedDiffuseTexture = nullptr;
}
