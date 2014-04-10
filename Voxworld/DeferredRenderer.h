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
#include "PointLightShader.h"
#include "SpotLightShader.h"
#include "SpotLight.h"
#include "MaterialNode.h"
#include "GBuffer.h"
#include <SDL.h>

class Frustum;

class DeferredRenderer :
	public Renderer
{
public:
	DeferredRenderer(int p_WindowWidth, int p_WindowHeight);
	virtual ~DeferredRenderer(void);

	virtual void init(void);
	virtual void shutDown(void);
	virtual void beginRenderCycle(RenderMode p_Mode);
	virtual void endRenderCycle(void);
	virtual void beginUIPhase(void);
	virtual void begin(void);
	virtual void end(void);

	virtual void render(SceneNode* p_Node);
	virtual void render(MeshNode* p_Mesh);
	virtual void render(TransformNode* p_Transform);
	virtual void render(TextureNode* p_TextureNode);
	virtual void render(CameraNode* p_CameraNode);
	virtual void render(LightNode* p_PointLightNode);
	virtual void render(SpotLight* p_SpotLightNode);
	virtual void render(MaterialNode* p_Material);

	virtual void setTransparencyMode(const bool p_Transparency){;}
	virtual SDL_Window* getWindow(void);

	// Write all the geometry data to different buffers (similar to forward rendering), take the vector of dataSets as a parameter
	virtual void geometryPass(std::vector<StandardDataSet> &p_DataList);
	virtual void geometryPass(std::vector<UIDataSet> &p_DataList);
	virtual void stencilPass();		// Use depth testing to generate stencil buffer
	virtual void dirLightPass();	// Render a quad (so all the fragments on screen gets affected) for direction light calculations
	virtual void pointLightPass(PointLightDataSet &p_lightData);	// Render a sphere for each point light
	virtual void spotLightPass(SpotLightDataSet &p_lightData);		// Render a cone for each spot light
	virtual void skyboxPass();		// If we use a skybox, it will be rendered here, so it's placement is correct and is not affected by the lights
	virtual void finalPass();		// Copy the final image to the screen (instead of an older method of rendering a fullscreen sized quad)
	virtual void updateViewFrustum(); //update the frustum based on the view and projection matrices

	virtual Frustum* getFrustum(){return m_Frustum;}

private:
	SDL_GLContext m_Context;
	SDL_Window	*m_Window;
	Material	*m_CurrentMaterial;
	GBuffer		*m_GBuffer;
	Frustum		*m_Frustum;
	RenderMode	 m_CurrentMode;

	std::vector<LightStruct> m_Lights;
	std::vector<Shader*>	 m_Shaders;
	PointLightShader		*m_PointLightShader;
	SpotLightShader			*m_SpotLightShader;
	Shader					*m_CurrentShader,
							*m_StencilPassShader;
	MeshNode				*m_PointLightMesh,
							*m_SpotLightMesh;

	//status flags, used for shader selection and error checking
	bool	m_Textures[4],
			m_Matrices[4],
			m_MeshFlag,
			m_UI_Phase;
	float	m_CurrentBRadius;

	glm::vec3	m_ModelPosition;
	glm::mat3	m_NormalMatrix;
	glm::mat4	m_CurrentViewMatrix,
				m_CurrentModelMatrix,
				m_ProjectionMatrix,
				m_OrthoProjectionMatrix,
				m_WorldTransform;

	GLuint	m_CurrentMesh,
			m_CurrentMeshVerts,
			m_CurrentDiffuse,
			m_CurrentEmissiveMap,
			m_CurrentNormalMap,
			m_CurrentHeightMap;

	std::vector<StandardDataSet>	m_DataList;
	std::vector<UIDataSet>			m_UIDataList;
	std::vector<StandardDataSet>	m_CulledList;
	std::vector<PointLightDataSet>	m_PointLightList;
	std::vector<SpotLightDataSet>	m_SpotLightList;

	void cullDataSet(std::vector<StandardDataSet> p_Unculled, std::vector<StandardDataSet> p_Culled, glm::mat4& p_ViewMatrix);
	bool frustumCheck(StandardDataSet p_DataSet);
};

