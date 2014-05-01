#pragma once
#include <memory>
#include <SDL.h>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "MaterialNode.h"
#include "Shader.h"
#include "GeneralUtils.h"
class MeshNode;
class TransformNode;
class TextureNode;
class CameraNode;
class LightNode;
class SpotLight;
class MaterialNode;
class Frustum;
class ParticleSystem;

enum RenderMode
{
	FILLED, //normal rendering mode
	WIREFRAME, //wireframe only
	DEBUG //draws buffer contents instead of final frame
};
enum MatrixType
{
	PROJECTION=0,
	MODEL,
	VIEW,
	NORMALMATRIX
};
//simple data set used to draw scene objects from standard camera persective;
struct StandardDataSet
{
	//mesh Data
	GLuint MeshLocation;
	GLuint MeshVertCount;
	//texture locations
	GLuint DiffuseMapLocation;
	GLuint NormalMapLocation;
	GLuint EmissiveMapLocation;
	GLuint HeightLocation;
	GLuint SpecularLocation;
	Material* Material;
	Shader* SelectedShader;
	//non camera related transform matrices
	glm::mat4 ModelMatrix;
	glm::mat3 normalMatrix;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	float boundingRadius;
	glm::vec3 modelPosition;
	glm::mat4 worldTransform;
};
struct UIDataSet
{
	//mesh Data
	GLuint MeshLocation;
	GLuint MeshVertCount;
	//texture locations
	GLuint DiffuseMapLocation;
	Material* Material;
	Shader* SelectedShader;
	//non camera related transform matrices
	glm::mat4 ModelMatrix;
	glm::mat3 normalMatrix;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
};
//simple stucture for storing cubemap data which will be written to, and read from.
struct EnvironmentMapDataSet
{
	glm::mat4 cubeViewMatrices[6];
	GLuint renderTargetTextures[6];
	GLuint CubeMapLocation;
};
// data structure for storing data to generate omno directional depth maps
struct CubeShadowmapDataSet
{
	glm::mat4 cubeViewMatrices[6];
	GLuint renderTargetTextures[6];
	GLuint CubeMapLocation;
};
//used to generate spotlight based depth maps
struct SpotLightShadowDataSet
{
	GLuint renderTargetTexture;
	glm::mat4 depthMapLocation;
};
struct PointLightDataSet
{
	glm::mat4 lightModel;
	glm::vec3 worldPosition;
	glm::vec3 attenuation;
	glm::vec3 colour;
	float ambientI;
	float diffuseI;
	float SpecI;
	float SpecP;
	PointLightDataSet(	glm::mat4 p_lightModel,	glm::vec3 p_worldPosition,	
						glm::vec3 p_colour,		glm::vec3 p_attenuation,	
						float p_ambientI,		float p_diffuseI,			
						float p_SpecI,			float p_SpecP):
						lightModel(p_lightModel),	worldPosition(p_worldPosition), colour(p_colour),	
						ambientI(p_ambientI),		diffuseI(p_diffuseI),			SpecI(p_SpecI),		
						SpecP(p_SpecP),				attenuation(p_attenuation){;}
	glm::vec3 calcLightScale()
	{
		float scale = (Utils::GeneralUtils::max(Utils::GeneralUtils::max(colour.r, colour.g), colour.b) * diffuseI);
		scale = (3.0f * sqrtf(scale) / attenuation.z);
		//return glm::vec3(scale, scale, scale);
		return glm::vec3(5000.0f);
	}
};
struct SpotLightDataSet
{
	glm::mat4 lightModel;
	glm::vec3 worldPosition;
	glm::vec3 attenuation;
	glm::vec3 colour;
	glm::vec3 worldDirection;
	float cutoffAngle;
	float ambientI;
	float diffuseI;
	float SpecI;
	float SpecP;
	SpotLightDataSet(	glm::vec3 p_worldPosition,		glm::vec3 p_colour,		glm::vec3 p_attenuation,
						glm::vec3 p_worldDirection,		float p_cutoffAngle,	float p_ambientI,	
						float p_diffuseI,				float p_SpecI,			float p_SpecP,
						glm::mat4 &p_lightModel):
						worldPosition(p_worldPosition), colour(p_colour),		worldDirection(p_worldDirection),
						cutoffAngle(p_cutoffAngle),		ambientI(p_ambientI),	diffuseI(p_diffuseI),
						SpecI(p_SpecI),					SpecP(p_SpecP),			attenuation(p_attenuation),
						lightModel(p_lightModel){;}
	glm::vec3 calcLightScale()
	{
		//magic number: check later
		return glm::vec3(4000.0f, 4000.0f, 4000.0f);
	}
};

struct ParticleDataSet
{
	GLuint		VAO,
				numParticles;
	float		boundingRadius,
				pointSize,
				maxLifeTime;
	glm::mat4	MVP;
	glm::vec4	colour;
	ParticleDataSet(	GLuint p_VAO,		GLuint p_numParticles,	float p_boundingRadius,
						glm::mat4 &p_MVP,	glm::vec4 &p_colour,	float p_pointSize,		float p_maxLifeTime):
						VAO(p_VAO), numParticles(p_numParticles), boundingRadius(p_boundingRadius), MVP(p_MVP), colour(p_colour), pointSize(p_pointSize), maxLifeTime(p_maxLifeTime) { }
};

class Renderer
{
public:
	Renderer(int p_ScreenWidth, int p_ScreenHeight);
	virtual void init(void)=0;
	virtual void beginRenderCycle(RenderMode p_Mode)=0;
	virtual void endRenderCycle(void)=0;
	virtual void begin(void) = 0;
	virtual void end(void) = 0;
	virtual void beginUIPhase(void) = 0;
	virtual void render(SceneNode* p_Node)=0;
	virtual void render(MeshNode* p_Mesh)=0;
	virtual void render(TransformNode* p_Transform)=0;
	virtual void render(TextureNode* p_TextureNode)=0;
	virtual void render(CameraNode* p_CameraNode)=0;
	virtual void render(LightNode* p_LightNode)=0;
	virtual void render(SpotLight* p_LightNode)=0;
	virtual void render(MaterialNode* p_Material)=0;
	virtual void render(ParticleSystem* p_Particle)=0;
	virtual void setTransparencyMode(const bool p_Transparency)=0;
	virtual void updateViewFrustum() = 0;
	virtual Frustum* getFrustum() = 0;
	virtual SDL_Window* getWindow(void)=0;
	virtual void shutDown(void) = 0;
	virtual ~Renderer(void);
protected:
	int m_ScreenWidth, m_ScreenHeight;
};

