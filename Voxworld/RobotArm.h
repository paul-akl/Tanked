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
	void setDamaged(bool p_Switch){m_Damaged = p_Switch;}
	void addDamagedTexture(TextureNode* p_Texture)	{ m_DamagedTextureDiffuse = p_Texture;	}
	void addDamagedEmissive(TextureNode* p_Texture)	{ m_DamagedTextureEmissive = p_Texture; }
	void addDamagedNormal(TextureNode* p_Texture)	{ m_DamagedTextureNormal = p_Texture;	}
	void addDamagedSpecular(TextureNode* p_Texture)	{ m_DamagedTextureSpecular = p_Texture;	}
	virtual ~RobotArm(void);
private:
	bool m_Raised;
	TextureNode *m_DamagedTextureDiffuse,
				*m_DamagedTextureEmissive,
				*m_DamagedTextureNormal,
				*m_DamagedTextureSpecular;
	float m_ArmRaiseSpeed;
	bool m_Damaged;
	bool m_Left;
};

