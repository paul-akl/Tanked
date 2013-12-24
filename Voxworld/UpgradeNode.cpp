#include "UpgradeNode.h"


UpgradeNode::UpgradeNode(void)
{
}
void UpgradeNode::init()
{
}
void UpgradeNode::update(float p_DeltaTimeS)
{
	if(m_Timer <= 0.0f)
	{
		deactivate();
	}
	else
	{
		m_Timer -= p_DeltaTimeS;
	}
	SceneNode::update(p_DeltaTimeS);
}
void UpgradeNode::render(Renderer* p_Renderer)
{
	if(!m_Collected)
	{
		SceneNode::render(p_Renderer);
	}
}
UpgradeNode::~UpgradeNode(void)
{
}
