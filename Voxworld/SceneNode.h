#pragma once
#include <string>
#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>
//base scene graph node. All scene node types will be derived from this class
class MeshNode;
class TextureNode;
class TransformNode;
class Renderer;
static const float PI = 3.14159265f;
static const float PI_OVER180 = PI/180.0f;
static const float RAD_TO_DEG = 180.0f/PI; 
class SceneNode
{
public:
	SceneNode(void);
	void setName(std::string p_Name);
	const std::string getName(void);
	void setTransform(const glm::mat4& p_Matrix);
	const glm::mat4& getLocalTransform(void) const;
	void setParent(SceneNode* p_Parent);
	void addNode(SceneNode* p_Child);
	void addTransform(TransformNode* p_LocalTransform);
	void addMesh(MeshNode* p_Mesh);
	void addTexture(TextureNode* p_Texture);
	void setPosition(const glm::vec3 p_Position);
	bool isMoving();
	glm::vec3 getLocation(){return m_Position;}
	void setOrientation(const float p_OrientationDeg);
	float getOrientation(){return m_OrientationDeg;}
	SceneNode* search(std::string p_Target);
	const glm::mat4& getWorldTransform(void);
	virtual void render(Renderer* p_Renderer)=0;
	virtual void update(float p_DeltaTimeS)=0;
	void setVelocity(glm::vec3 p_Velocity);
	const glm::vec3 getVelocity();
	void stop();
	void activate (void);
	void deactivate(void);
	const bool isActive(void);
	virtual ~SceneNode(void);
protected:
	std::string m_Name;
	bool m_IsActive;
	SceneNode* m_Parent;
	bool m_IsMoving; //used to indicate if the tank is in motion
	TransformNode* m_LocalTransform;
	MeshNode* m_Mesh;
	TextureNode* m_Diffuse;
	std::vector<SceneNode*> m_Children;
	glm::mat4 m_WorldTransform;
	glm::vec3 m_Position;
	float m_OrientationDeg;
	glm::vec3 m_Velocity;
};

