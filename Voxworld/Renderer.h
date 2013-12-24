#pragma once
#include <memory>
#include <SDL.h>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "MaterialNode.h"
#include "Shader.h"
class MeshNode;
class TransformNode;
class TextureNode;
class CameraNode;
class LightNode;
class MaterialNode;
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
	GLuint SpecularMapLocation;
	GLuint DepthMapLocation;
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
class Renderer
{
public:
	Renderer(int p_ScreenWidth, int p_ScreenHeight);
	virtual void init(void)=0;
	virtual void beginRenderCycle(RenderMode p_Mode)=0;
	virtual void endRenderCycle(void)=0;
	virtual void begin(void) = 0;
	virtual void end(void) = 0;
	virtual void render(MeshNode* p_Mesh)=0;
	virtual void render(TransformNode* p_Transform)=0;
	virtual void render(TextureNode* p_TextureNode)=0;
	virtual void render(CameraNode* p_CameraNode)=0;
	virtual void render(LightNode* p_LightNode)=0;
	virtual void render(MaterialNode* p_Material)=0;
	virtual void setTransparencyMode(const bool p_Transparency)=0;
	virtual SDL_Window* getWindow(void)=0;
	virtual void shutDown(void) = 0;
	virtual ~Renderer(void);
protected:
	int m_ScreenWidth, m_ScreenHeight;
};

