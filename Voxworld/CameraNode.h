#pragma once
#include "scenenode.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Flexible camera class, using Euler angles to create a view transform matrix								 //
// Example Use:																								 //
// camera.MoveForward(moveSpeed*deltaTime);																	 //
// camera.RotatePitch(rotateSpeed*deltaTime);																 //
// camera.getViewTransform(ViewMatrix);																		 //
// camera.MoveTo(targetPosition);																			 //
// camera.LookAt(object.getPosition(),viewingDistance);		                                                 //
//																											 //
// To rotate around an object in third person:																 //
// camera.Reset();									all previous settings are useless now					 //
// camera.RotateYaw(object.getHorizOrientation);	camera must be set to look the same direction as object	 //
// camera.RotatePitch(-preferredVerticalAngle);		Looking down onto object								 //
// camera.LookAt(object.getPosition(),preferredFollowDistance);			set camera to look at object		 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//simple struct for storing Euler angles, and position
struct EulerAngle
{
	glm::vec3 m_Position;
	float m_RollAngle;
	float m_PitchAngle;
	float m_YawAngle;
};
//macro defining value of PI, divided by 180 degrees, for converting degrees to radians
#define PIOVER180 0.017453293;

class CameraNode :
	public SceneNode
{
public:
	CameraNode(void);
	void rotateYaw(const float& p_Angle);
	void rotatePitch(const float& p_Angle);
	void rotateRoll(const float& p_Angle);
	void moveForward(const float& p_Amount);
	void moveRight(const float& p_Amount);
	void moveUp(const float& p_Amount);
	void moveX(const float& p_Amount);
	void moveY(const float& p_Amount);
	void moveZ(const float& p_Amount);
	void MoveTo(glm::vec3 p_NewPosition);
	void LookAt(const glm::vec3& p_Point, const float& p_Distance);
	//this method levels out the camera, zeroing vertical angle and roll angle
	void LevelCamera();
	//overridable methods
	virtual void update(float p_DeltaTimeMS);
	virtual void render(Renderer* p_Renderer);
	//completely resets camera
	void init();
	void getViewMatrix(glm::mat4& p_ViewMatrix, bool p_RotationOnly);
	virtual ~CameraNode(void);
private:
		//stored Euler orientation
	EulerAngle m_Orientation;
	//directions used for movement
	glm::vec3 m_Direction;
	glm::vec3 m_Right;
};

