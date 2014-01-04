#pragma once
#include "scenenode.h"

class RobotArm :
	public SceneNode
{
public:
	RobotArm(void);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	void raiseArm();
	void lowerArm();
	void setLeft();
	const bool& armRaised();
	void addDamagedTexture(TextureNode* p_Texture){m_DamagedDiffuseTexture = p_Texture;}
	virtual ~RobotArm(void);
private:
	bool m_Raised;
	TextureNode* m_DamagedDiffuseTexture;
	float m_ArmRaiseSpeed;
	bool m_Damaged;
	bool m_Left;
};

