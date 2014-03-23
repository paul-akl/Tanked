#pragma once
#include "scenenode.h"
class RobotHead :
	public SceneNode
{
public:
	RobotHead(void);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	void addDamagedTexture(TextureNode* p_Texture);
	void setDamaged(bool p_Switch){m_Damaged = p_Switch;}
	void LookAt(const glm::vec3& p_Target);
	virtual ~RobotHead(void);
protected:
	void turnLeft(float p_DeptaTimeS);
	void turnRight(float p_DeltaTimeS);
	float m_TargetOrientation;
	TextureNode* m_DamagedDiffuseTexture;
	bool m_Damaged;
	float m_TurnSpeed;
	bool m_Turning;
};

