#pragma once
#include "scenenode.h"
class RobotHead :
	public SceneNode
{
public:
	RobotHead(void);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	void addDamagedTexture(TextureNode* p_Texture)	{ m_DamagedTextureDiffuse = p_Texture;	}
	void addDamagedEmissive(TextureNode* p_Texture)	{ m_DamagedTextureEmissive = p_Texture; }
	void addDamagedNormal(TextureNode* p_Texture)	{ m_DamagedTextureNormal = p_Texture;	}
	void addDamagedSpecular(TextureNode* p_Texture)	{ m_DamagedTextureSpecular = p_Texture;	}
	void setDamaged(bool p_Switch){m_Damaged = p_Switch;}
	void LookAt(const glm::vec3 p_Target);
	float getTargetOrientation(){return m_TargetOrientation;}
	void setTargetOrientation(float p_TargetOrientation){m_TargetOrientation=p_TargetOrientation;}
	void rotate(float p_Rotation);
	void turnLeft(float p_DeptaTimeS);
	void turnRight(float p_DeltaTimeS);
	virtual ~RobotHead(void);
protected:
	float m_TargetOrientation;
	TextureNode *m_DamagedTextureDiffuse,
				*m_DamagedTextureEmissive,
				*m_DamagedTextureNormal,
				*m_DamagedTextureSpecular;
	glm::vec3 m_LookTarget;
	bool m_Damaged;
	float m_TurnSpeed;
	bool m_Turning;
};

