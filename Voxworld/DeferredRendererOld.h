//////////////////////////////////////////////////////////////////////////////////
//Within the application beginRenderCycle will be called for each game object   //
//then all calls to render nodes												//	
//then call end for each object	which builds up the data sets for the gbuffer	//
//once all objects have been through this cycle endRenderCycle is called		//	
//which loops through the data sets and renders them to the gbuffer				//		
//--still needs lighting pass added												//
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "renderer.h"
#include "Shader.h"
#include "LightNode.h"
#include "MaterialNode.h"
#include "GBuffer.h"
#include <SDL.h>

class DeferredRenderer :
	public Renderer
{
public:
	DeferredRenderer(int p_WindowWidth, int p_WindowHeight);
	virtual ~DeferredRenderer(void);
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

	// Write all the geometry data to different buffers (similar to forward rendering), take the vector of dataSets as a parameter
	virtual void geometryPass(std::vector<StandardDataSet> &p_DataList);	
	virtual void stencilPass();		// Use depth testing to generate stencil buffer
	virtual void dirLightPass();	// Render a quad (so all the fragments on screen gets affected) for direction light calculations
	virtual void pointLightPass();	// Render a sphere for each point light
	virtual void spotLightPass();	// Render a cone for each spot light
	virtual void skyboxPass();		// If we use a skybox, it will be rendered here, so it's placement is correct and is not affected by the lights
	virtual void finalPass();		// Copy the final image to the screen (instead of an older method of rendering a fullscreen sized quad)

private:
	SDL_Window* m_Window;
	SDL_GLContext m_Context;
	Shader *m_CurrentShader,
			*m_StencilPassShader,
			*m_DirLightShader,
			*m_PointLightShader;
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
	Material* m_CurrentMaterial;
	GBuffer * m_GBuffer;
	std::vector<StandardDataSet> m_DataList;
};

