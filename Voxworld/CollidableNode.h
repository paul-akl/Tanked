#pragma once
#include "scenenode.h"
enum CollidableType
{
	PLAYER,
	ENEMY,
	ENEMY_ROBOT,
	PROJECTILE,
	SCENERY,
	COLLECTABLE,
	MISC,
	AREA_EFFECT,
	PHASING
};
enum BoundaryType
{
	CIRCLE,
	AAB,
	OBB,
	POINT,
	RAY,
};
class CollidableNode :
	public SceneNode
{
public:
	CollidableNode(void);
	void setRadius(const float p_Radius){m_Radius = p_Radius;}
	void setType(CollidableType p_Type){m_Type=p_Type;}
	CollidableType getType(){return m_Type;}
	float getRadius(){return m_Radius;}
	virtual void render(Renderer* p_Renderer)=0;
	virtual void update(float p_DeltaTimeS)=0;
	bool isRecursive(){return m_IsRecursive;}
	void setRecursive(bool p_Switch){m_IsRecursive=p_Switch;}
	void setBoundaryType(BoundaryType p_Type){m_BoundaryType = p_Type;}
	BoundaryType getBoundaryType(){return m_BoundaryType;}
	float getWidth(){return m_Width;}
	float getLength(){return m_Length;}
	void setWidth(float p_Width){m_Width = p_Width;}
	void setLength(const float p_Length){m_Length = p_Length;}
	virtual ~CollidableNode(void);
protected:
	float m_Radius;
	CollidableType m_Type;
	BoundaryType m_BoundaryType;
	bool m_IsRecursive;
	int m_NumSubSections;
	float m_Width;
	float m_Length;
	
};

