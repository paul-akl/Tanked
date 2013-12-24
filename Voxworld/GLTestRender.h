#pragma once
#include "renderer.h"
#include "Shader.h"
#include "LightNode.h"
#include "MaterialNode.h"
#include <SDL.h>


class GLTestRender :
	public Renderer
{
public:
	GLTestRender(int p_WindowWidth, int p_WindowHeight);
	virtual void init(void);
	virtual void beginRenderCycle(RenderMode p_Mode);
	virtual void endRenderCycle(void);
	virtual void begin(void);
	virtual void end(void);
	virtual void render(MeshNode* p_Mesh);
	virtual void render(TransformNode* p_Transform);
	virtual void render(TextureNode* p_TextureNode);
	virtual void render(CameraNode* p_CameraNode);
	virtual void render(LightNode* p_CameraNode);
	virtual void render(MaterialNode* p_Material);
	virtual void setTransparencyMode(const bool p_Transparency){;}
	virtual SDL_Window* getWindow(void);
	virtual void shutDown(void);
	virtual ~GLTestRender(void);
private:
	SDL_Window* m_Window;
	SDL_GLContext m_Context;
	Shader* m_CurrentShader;
	std::vector<Shader*> m_Shaders;
	std::vector<LightStruct> m_Lights;
	//status flags, used for shader selection and error checking
	bool m_Textures[4];
	bool m_Matrices[4];
	bool m_MeshFlag;
	glm::mat4 m_CurrentViewMatrix;
	glm::mat4 m_CurrentModelMatrix;
	glm::mat3 m_NormalMatrix;
	glm::mat4 m_ProjectionMatrix;
	GLuint m_CurrentMesh;
	GLuint m_CurrentMeshVerts;
	GLuint m_CurrentDiffuse;
	GLuint m_CurrentSpecMap;
	GLuint m_CurrentNormalMap;
	GLuint m_CurrentDepthMap;
	RenderMode m_CurrentMode;
	float m_ScreenWidth;
	float m_ScreenHeight;
	Material* m_CurrentMaterial;
};

