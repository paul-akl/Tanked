#include "QTNode.h"


QTNode::QTNode(void)
{
	m_Children[0] = nullptr;
	m_Children[1] = nullptr;
	m_Children[2] = nullptr;
	m_Children[3] = nullptr;
}


QTNode::~QTNode(void)
{
	if(m_Children[0]!=nullptr)
		delete m_Children[0];
	if(m_Children[1]!=nullptr)
		delete m_Children[1];
	if(m_Children[2]!=nullptr)
		delete m_Children[2];
	if(m_Children[3]!=nullptr)
		delete m_Children[3];
	m_Objects.clear();
}
