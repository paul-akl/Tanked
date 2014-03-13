#include "SceneNode.h"
#include "Renderer.h"
#include "TransformNode.h"
#include "TextureNode.h"
#include "MeshNode.h"
SceneNode::SceneNode(void)
{
	m_Parent = nullptr;
	m_Mesh = nullptr;
	m_Diffuse = nullptr;
	m_LocalTransform = nullptr;
	m_IsActive = true;
	m_OrientationDeg = 0.0f;
	m_Position = glm::vec3(0.0f);
	m_IsMoving = false;
	m_RenderRadius = glm::vec3(0.0f);
}
bool SceneNode::isMoving()
{
	return m_IsMoving;
}
void SceneNode::stop()
{
	m_Velocity = glm::vec3(0.0f);
}
void SceneNode::setVelocity(glm::vec3 p_Velocity)
{
	m_Velocity = p_Velocity;
}
const glm::vec3 SceneNode::getVelocity()
{
	return m_Velocity;
}
void SceneNode::setPosition(const glm::vec3 p_Position)
{
	m_Position = p_Position;
}
void SceneNode::addTransform(TransformNode* p_LocalTransform)
{
	m_LocalTransform = p_LocalTransform;
	m_LocalTransform->setParent(this);
}
void SceneNode::addMesh(MeshNode* p_Mesh)
{
	m_Mesh = p_Mesh;
	addNode(p_Mesh);
}
void SceneNode::addTexture(TextureNode* p_Texture)
{
	m_Diffuse = p_Texture;
	addNode(p_Texture);
}
const glm::mat4& SceneNode::getWorldTransform(void)
{
	return m_WorldTransform;
}
void SceneNode::activate(void)
{
	m_IsActive=true;
}
void SceneNode::deactivate(void)
{
	m_IsActive=false;
}
const bool SceneNode::isActive(void)
{
	return m_IsActive;
}
void SceneNode::setName(std::string p_Name)
{
	m_Name = p_Name;
}
const std::string SceneNode::getName(void)
{
	return m_Name;
}
void SceneNode::setParent(SceneNode* p_Parent)
{
	m_Parent = p_Parent;
}
void SceneNode::update(float p_DeltaTime)
{
	//calculate transform matrices
	if(m_Parent == nullptr)
	{
		//if node is root, then local transform is this object's world transform
		//if (m_LocalTransform != nullptr)
		m_WorldTransform = m_LocalTransform->getLocalTransform();
	}
	else
	{
		//if (m_LocalTransform == nullptr)
			//m_WorldTransform = m_Parent->getWorldTransform();
		//else
		
		//if not root, then all transforms are relative to parent's world transform
		glm::mat4 p_WorldTransform = m_Parent->getWorldTransform();
		glm::mat4 p_Local = m_LocalTransform->getLocalTransform();
		m_WorldTransform = p_WorldTransform*p_Local;
		
	}
	//default behaviour: update all children
	if(m_IsActive)
	{
		for(size_t i = 0; i < m_Children.size();i++)
		{
			if(m_Children[i]!= nullptr)
			{
				if(m_Children[i]->isActive())
					m_Children[i]->update(p_DeltaTime);
			}
		}
	}
}
void SceneNode::render(Renderer* p_Renderer)
{
	//default behaviour render all children
	if(m_IsActive)
	{
		// if this node has a texture, mesh, and transform, then we can render it
		if(m_Diffuse!=nullptr && m_Mesh!=nullptr && m_LocalTransform!=nullptr)
		{
			//so long as all those nodes are active
			if(m_Diffuse->isActive() && m_Mesh->isActive() && m_LocalTransform->isActive())
			{
				p_Renderer->begin();
				m_Diffuse->render(p_Renderer);
				m_LocalTransform->render(p_Renderer);
				//any other texture render calls go in here
				m_Mesh->render(p_Renderer);
				p_Renderer->end();
			}
		}
		for(size_t i = 0; i < m_Children.size();i++)
		{
			if(m_Children[i]!= nullptr)
			{
				if(m_Children[i]->isActive())
					if(m_Children[i]->getName()!=m_Mesh->getName())
						if(m_Children[i]->getName()!=m_Diffuse->getName())
							if(m_Children[i]->getName()!=m_LocalTransform->getName())
							{
								m_Children[i]->render(p_Renderer);
							}
			}
		}

	}
}
void SceneNode::addNode(SceneNode* p_Child)
{
	//add node to child list and set up reverse lookup
	bool v_Found = false;
	size_t v_I = 0;
	//look for an empty slot
	while(!v_Found && v_I < m_Children.size())
	{
		if(m_Children[v_I]==nullptr)
		{
			m_Children[v_I]=p_Child;
			p_Child->setParent(this);
			v_Found = true;
		}
		v_I++;
	}
	//if no empty slots, create one
	if(!v_Found)
	{
		m_Children.push_back(p_Child);
		p_Child->setParent(this);
	}
	
}
SceneNode* SceneNode::search(std::string p_Target)
{
	//performs a depth first search, and returns result
	if(m_Name != p_Target)
	{	
		//linear search through children
		for (size_t i = 0; i < m_Children.size();i++)
		{
			if(m_Children[i]!=nullptr)
			{
				//if child name matches search parameter
				if(m_Children[i]->getName()==p_Target)
				{
					//return that child, node found
					return m_Children[i];
				}
				//if child name does not match parameter
				else
				{
					//activate recursive search, and return result if we get a positive match
					if(m_Children[i]->search(p_Target)!=nullptr)
					{
						return m_Children[i]->search(p_Target);
					}
					//if still not found, iterate to next child
					else
					{
						continue;
					}
				}
			}
			else
			{
				continue;
			}
		}
		//if we have reached this stage, this structure branch does not hold node, so return a null pointer
		return nullptr;
	}
	else
	{
		return this;
	}
	return nullptr;
}
void SceneNode::setOrientation(const float p_OrientationDeg)
{
	m_OrientationDeg = p_OrientationDeg;
}
SceneNode::~SceneNode(void)
{
	//almost all references are no unique, and managed elsewhere, so delete nothing
	m_Parent=nullptr;
	for (size_t i = 0; i < m_Children.size();i++)
	{
		m_Children[i] = nullptr;
	}
	m_Mesh=nullptr;
	//local transforms are unique, so need deleted
	delete m_LocalTransform;
	m_Diffuse = nullptr;
}
