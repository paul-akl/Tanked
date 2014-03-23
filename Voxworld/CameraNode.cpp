#include "CameraNode.h"
#include "GLTestRender.h"
#include <glm\gtc\matrix_transform.hpp>

CameraNode::CameraNode(void)
{
	//call init
	init();
}
void CameraNode::init()
{
	//initialize all values to zero
	m_Orientation.m_PitchAngle = 0.0f;
	m_Orientation.m_YawAngle = 0.0f;
	m_Orientation.m_RollAngle = 0.0f;
	m_Orientation.m_Position = glm::vec3(0.0);
	update(0.0f);
}
void CameraNode::rotateYaw(const float& p_Angle)
{
	//convert to radians and increment stored horizontal angle
	m_Orientation.m_YawAngle+=p_Angle*(float)PIOVER180;
	update(0.0f);
}
void CameraNode::rotatePitch(const float& p_Angle)
{
	//conevrt to radians and increment stored vertical angle
	m_Orientation.m_PitchAngle+=p_Angle*(float)PIOVER180;
	update(0.0f);
}
void CameraNode::rotateRoll(const float& p_Angle)
{
	//convert to radians and increment stored roll angle
	m_Orientation.m_RollAngle+=p_Angle*(float)PIOVER180;
	update(0.0f);
}
void CameraNode::MoveTo(glm::vec3 p_NewPosition)
{
	m_Orientation.m_Position = p_NewPosition;
}
void CameraNode::moveForward(const float& p_Amount)
{
	//move camera on it's local forward axis
	m_Orientation.m_Position+=m_Direction*p_Amount;
}
void CameraNode::moveRight(const float& p_Amount)
{
	//move camera on its own left/right axis
	m_Orientation.m_Position+=m_Right* p_Amount;
}
void CameraNode::moveUp(const float& p_Amount)
{
	//move camera along its own up/down axis
	m_Orientation.m_Position+=glm::cross(m_Right,m_Direction)*p_Amount;
}
void CameraNode::LookAt(const glm::vec3& p_Point, const float& p_Distance)
{
	MoveTo(p_Point);
	moveForward(p_Distance);
}
void CameraNode::moveX(const float& p_Amount)
{
	//move camera along cartesian x axis
	m_Orientation.m_Position+=glm::vec3(1.0f,0.0f,0.0f) * p_Amount; 
}
void CameraNode::moveY(const float& p_Amount)
{
	//move camera along cartesian y axis
	m_Orientation.m_Position+=glm::vec3(0.0f,1.0f,0.0f) * p_Amount; 
}
void CameraNode::moveZ(const float& p_Amount)
{
	//move camera along cartesian z axis
	m_Orientation.m_Position+=glm::vec3(0.0f,0.0f,1.0f) * p_Amount;
}
void CameraNode::update(float p_DeltaTimeMS)
{
		//convert stored Euler angles into cartesian vectors, to calculate Forward direction
	m_Direction = glm::normalize(	glm::vec3(	cos(m_Orientation.m_PitchAngle) * sin(m_Orientation.m_YawAngle),
												sin(m_Orientation.m_PitchAngle),
												cos(m_Orientation.m_PitchAngle) * cos(m_Orientation.m_YawAngle)));
	//and perpendicular Right direction
	m_Right = glm::normalize(	glm::vec3(	sin(m_Orientation.m_YawAngle - 3.141592/2.0f),
											0.0f,
											cos(m_Orientation.m_YawAngle - 3.141592/2.0f)));
	//Up direction is only needed for view matrix generation, so no need to calculate it here.
}
void CameraNode::render(Renderer* p_Renderer)
{
	//visitor pattern call
	p_Renderer->render(this);
}
void CameraNode::getViewMatrix(glm::mat4& p_ViewMatrix, bool p_RotationOnly)
{
		//of all we want is the rotation matrix, 
	glm::vec3 v_Up = glm::cross(m_Right,m_Direction);
	if(p_RotationOnly)
	{
		p_ViewMatrix = glm::mat4(glm::mat3(glm::lookAt(m_Orientation.m_Position,m_Orientation.m_Position+m_Direction,v_Up)));
	}
	else
	{
		p_ViewMatrix =glm::lookAt(m_Orientation.m_Position,m_Orientation.m_Position+m_Direction,v_Up);
	}
}
CameraNode::~CameraNode(void)
{

}
