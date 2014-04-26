#include "DeferredRenderer.h"
#include "TextureNode.h"
#include "TransformNode.h"
#include "MeshNode.h"
#include "CameraNode.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GeneralUtils.h"
#include "Shader.h"
#include "Frustum.h"
#include "SpotLight.h"

DeferredRenderer::DeferredRenderer(int p_WindowWidth, int p_WindowHeight):Renderer(p_WindowWidth, p_WindowHeight)
{
		m_UI_Phase = false;
		m_Frustum = new Frustum();
		m_ProjectionMatrix = glm::perspective(60.0f, (float)m_ScreenWidth / (float)m_ScreenHeight, 1.0f, 400.0f);
}

void DeferredRenderer::begin(void)
{
}

void DeferredRenderer::updateViewFrustum()
{
	m_Frustum->deriveFrustum(m_CurrentViewMatrix, m_ProjectionMatrix);
}

void DeferredRenderer::beginRenderCycle(RenderMode p_Mode)
{
	// bind the GBuffer to write to
	m_GBuffer->enable();
	//clear colour buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TRIANGLES);
	m_CurrentMode = p_Mode;
	//if mesh data has been set
	if(m_MeshFlag)
	{
		//set rendering mode
		switch(m_CurrentMode)
		{
		case(WIREFRAME):
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_CULL_FACE);
				break;
			}
		case(DEBUG):
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_CULL_FACE);
				break;
			}
		case(FILLED):
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_CULL_FACE);
				break;
			}
		}
	}
	//un-needed as far as i'm aware. should be in end
	////--old comments--////
	//calls to render data sets go here
	//shadow maps first(render to texture)
	//environment maps
	
	
	//reset all flags for new rendering cycle
	for (size_t i = 0;i<4;i++)
	{
		m_Matrices[i] = false;
		m_Textures[i] = false;
	}
	
	m_OrthoProjectionMatrix = glm::ortho<float>(-1.0f,1.0f,-1.0f,1.0f);
	m_Matrices[PROJECTION] = true;
}
void DeferredRenderer::end(void)
{
	m_NormalMatrix = glm::inverse(glm::transpose(glm::mat3(m_CurrentViewMatrix*m_CurrentModelMatrix)));
	m_Matrices[NORMALMATRIX] = true;
	//calculating this within render to gbuffer
	//glm::mat4 modelview = m_CurrentViewMatrix*m_CurrentModelMatrix;
	int v_NumMatrices=0;
	int v_NumTextures=0;
	//select shader based on v_NumTextures

	if(!m_UI_Phase)
	{
		//Create Standard data set
		//Populate with all m_Current* data
		//add data set to list of data sets
		StandardDataSet v_DataSet;
		//mesh Data
		v_DataSet.MeshLocation = m_CurrentMesh;
		v_DataSet.MeshVertCount = m_CurrentMeshVerts;
		//texture locations
		// may need to add flags
		if(m_CurrentDiffuse != 0)
			v_DataSet.DiffuseMapLocation = m_CurrentDiffuse;
		else
			v_DataSet.DiffuseMapLocation = m_DiffuseTexture->getTexture();

		if(m_CurrentNormalMap != 0)
			v_DataSet.NormalMapLocation = m_CurrentNormalMap;
		else
			v_DataSet.NormalMapLocation = m_NormalTexture->getTexture();

		if(m_CurrentEmissiveMap != 0)
			v_DataSet.EmissiveMapLocation = m_CurrentEmissiveMap;
		else
			v_DataSet.EmissiveMapLocation = m_BlackTexture->getTexture();

		if(m_CurrentHeightMap != 0)
			v_DataSet.HeightLocation = m_CurrentHeightMap;
		else
			v_DataSet.HeightLocation = m_WhiteTexture->getTexture();

		//material
		//v_DataSet.Material = m_CurrentMaterial;
		//shader
		v_DataSet.SelectedShader = m_CurrentShader;
		//non camera related transform matrices
		v_DataSet.ModelMatrix = m_CurrentModelMatrix;
		v_DataSet.viewMatrix = m_CurrentViewMatrix;
		v_DataSet.normalMatrix = m_NormalMatrix;
		v_DataSet.projectionMatrix = m_ProjectionMatrix;
		v_DataSet.boundingRadius = m_CurrentBRadius;
		v_DataSet.modelPosition =glm::vec3( m_CurrentModelMatrix*glm::vec4(glm::vec3(0.0f),1.0f));

		m_DataList.push_back(v_DataSet);
	}
	else if(m_UI_Phase)
	{
		//Create data set for ui components
		UIDataSet v_UIDataSet;

		v_UIDataSet.MeshLocation = m_CurrentMesh;
		v_UIDataSet.MeshVertCount = m_CurrentMeshVerts;
		//texture locations
		// may need to add flags
		v_UIDataSet.DiffuseMapLocation = m_CurrentDiffuse;
		//material
		//v_UIDataSet.Material = m_CurrentMaterial;
		//shader
		v_UIDataSet.SelectedShader = m_UIShader;
		//non camera related transform matrices
		v_UIDataSet.ModelMatrix = m_CurrentModelMatrix;
		v_UIDataSet.viewMatrix = glm::mat4(1.0f);
		//v_UIDataSet.normalMatrix = m_NormalMatrix;
		v_UIDataSet.projectionMatrix = m_OrthoProjectionMatrix;

		m_UIDataList.push_back(v_UIDataSet);
	}


	
	// may need to move this
	for (size_t i = 0;i<4;i++)
	{
		m_Textures[i] = false;
	}

	m_Matrices[MODEL] = false;
	m_Matrices[NORMALMATRIX] = false;
	m_CurrentDiffuse	 = 0;
	m_CurrentNormalMap	 = 0;
	m_CurrentEmissiveMap = 0;
	m_CurrentHeightMap	 = 0;
}
//this is called only when every object to be rendered has called its own render function
void DeferredRenderer::endRenderCycle(void)
{
	m_GBuffer->initFrame();		// Required to clear final buffer
	//somewhere here will be a frustum check
	cullDataSet(m_DataList, m_CulledList, m_CurrentViewMatrix); 
	geometryPass(m_CulledList);	// Render geometry of objects, pass vector of dataSets for current frame
	
	glEnable(GL_STENCIL_TEST);
	for(std::vector<PointLightDataSet>::size_type i = 0; i < m_PointLightList.size(); i++)
	{
		stencilPass(m_PointLightList[i].lightModel, *m_PointLightMesh);
		pointLightPass(m_PointLightList[i]);
	}
	glDisable(GL_STENCIL_TEST);
	for(std::vector<SpotLightDataSet>::size_type i = 0; i < m_SpotLightList.size(); i++)
	{
		//stencilPass(m_SpotLightList[i].lightModel, *m_SpotLightMesh);
		//glDisable(GL_STENCIL_TEST);
		spotLightPass(m_SpotLightList[i]);
	}
	glBindVertexArray(0);
	
	blurPass(GBuffer::GBufferEmissive, GBuffer::GBufferFinal, 3, 0.6f, -0.1f);

    glCullFace(GL_BACK);
	glDisable(GL_BLEND);

	UIPass(m_UIDataList);	// Render UI objects, pass vector of UI dataSets for current frame

	finalPass();

	// Clear current frame data sets, to get ready for the next frame
	m_DataList.clear();			
	m_CulledList.clear();
	m_UIDataList.clear();
	m_PointLightList.clear();
	m_SpotLightList.clear();

	m_Matrices[VIEW] = false;
	m_Matrices[PROJECTION] = false;
	m_UI_Phase = false;

	SDL_GL_SwapWindow(m_Window); // swap buffers
}
void DeferredRenderer::beginUIPhase(void)
{
	m_UI_Phase = true;
}

void DeferredRenderer::render(SceneNode* p_Node)
{
	m_CurrentBRadius = p_Node->getBoundingRadius();
	m_ModelPosition = p_Node->getLocation();
	m_WorldTransform = p_Node->getWorldTransform();
}

void DeferredRenderer::render(TextureNode* p_TextureNode)
{
	switch(p_TextureNode->getTextureType())
	{
	case(DIFFUSE):
		{
			m_CurrentDiffuse = p_TextureNode->getTexture(); 
			m_Textures[DIFFUSE] = true;
			break;
		}
		case(NORMAL):
		{
			m_CurrentNormalMap = p_TextureNode->getTexture();
			m_Textures[NORMAL] = true;
			break;
		}
		case(EMISSIVE):
		{
			m_CurrentEmissiveMap = p_TextureNode->getTexture();
			m_Textures[EMISSIVE] = true;
			break;
		}
		case(HEIGHT):
		{
			m_CurrentHeightMap = p_TextureNode->getTexture();
			m_Textures[HEIGHT] = true;
			break;
		}
	}
}
void DeferredRenderer::render(TransformNode* p_Transform)
{
	//this crashes when complete. Possibly recursion based due to shared pointers. May have to 
	//change visitor patter to raw pointers.
	m_CurrentModelMatrix = p_Transform->getWorldTransform();
	m_Matrices[MODEL] = true;
}
void DeferredRenderer::render(MeshNode* p_Mesh)
{
	m_CurrentMesh = p_Mesh->getMeshLocation();
	m_CurrentMeshVerts = p_Mesh->getNumVerts();
	m_MeshFlag = true;
}
void DeferredRenderer::render(MaterialNode* p_Material)
{
	m_CurrentMaterial = p_Material->getMaterialData();
}
void DeferredRenderer::render(CameraNode* p_CameraNode)
{
	p_CameraNode->getViewMatrix(m_CurrentViewMatrix,false);
	//glm::mat4 view = p_CameraNode->getLocalTransform();
	m_CameraPosition = p_CameraNode->getLocation();
	m_Matrices[VIEW] = true;
}
void DeferredRenderer::render(LightNode* p_PointLightNode)
{
	m_PointLightList.push_back(PointLightDataSet(
		p_PointLightNode->getLightModel(),			glm::vec3(p_PointLightNode->getWorldTransform()*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),		p_PointLightNode->getColour(),				
		p_PointLightNode->getAttenuation(),			p_PointLightNode->getAmbientIntensity(),	p_PointLightNode->getDiffuseIntensity(),	
 		p_PointLightNode->getSpecularIntensity(),	p_PointLightNode->getSpecularPower() ));
}
void DeferredRenderer::render(SpotLight* p_SpotLightNode)
{
	m_SpotLightList.push_back(SpotLightDataSet(
		p_SpotLightNode->getTransformedPosition(),	p_SpotLightNode->getColour(),			 p_SpotLightNode->getAttenuation(),
		p_SpotLightNode->getTransformedDirection(),	p_SpotLightNode->getCutoffAngle(),		 p_SpotLightNode->getAmbientIntensity(),
		p_SpotLightNode->getDiffuseIntensity(),		p_SpotLightNode->getSpecularIntensity(), p_SpotLightNode->getSpecularPower(),
		p_SpotLightNode->getLightModel()	));
}

SDL_Window* DeferredRenderer::getWindow(void)
{
	return m_Window;
}
void DeferredRenderer::init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize video
		Utils::GeneralUtils::exitFatalError("SDL Init error");

    // Request an OpenGL 3.0 context.
	
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); 
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // double buffering on
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // 8 bit alpha buffering
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // Turn on x4 multisampling anti-aliasing (MSAA)
    // Create 800x600 window
   m_Window = SDL_CreateWindow("SDL/GLM/OpenGL Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_ScreenWidth, m_ScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	if (!m_Window) // Check window was created OK
        Utils::GeneralUtils::exitFatalError("cannot create window");

    m_Context = SDL_GL_CreateContext(m_Window); // Create opengl context and attach to window
    SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate
	//this is needed for windows 
	glewExperimental = GL_TRUE;
	//initialise glew
	GLenum err = glewInit();
	if (GLEW_OK != err) { // glewInit failed, something is seriously wrong
		Utils::GeneralUtils::exitFatalError("GlewInit error");
	}
	//output version
	printf((const char*)glGetString(GL_VERSION));
	
	// Clear color needs to be black, for light blending
	glClearColor(0.0f,0.0f,0.0f,1.0f);

	//repeat next 4 lines for each additional shader
	m_CurrentShader = nullptr;
	m_GeometryShader = new Shader();
	m_GeometryShader->initShader("Shaders\\geometryPass.vert","Shaders\\geometryPass.frag");
	m_UIShader = new Shader();
	m_UIShader->initShader("Shaders\\UIPass.vert","Shaders\\UIPass.frag");

	m_BlurVerticalShader = new GaussianBlurShader();
	m_BlurVerticalShader->initShader("Shaders\\gaussianBlurVertical.vert", "Shaders\\gaussianBlurVertical.frag");
	m_BlurHorizontalShader = new GaussianBlurShader();
	m_BlurHorizontalShader->initShader("Shaders\\gaussianBlurHorizontal.vert", "Shaders\\gaussianBlurHorizontal.frag");
	m_StencilPassShader = new Shader();
	m_StencilPassShader->initShader("Shaders\\stencilPass.vert", "Shaders\\stencilPass.frag");
	m_PointLightShader = new PointLightShader();
	m_PointLightShader->initShader("Shaders\\pointLightPass.vert", "Shaders\\pointLightPass.frag");
	m_SpotLightShader = new SpotLightShader();
	m_SpotLightShader->initShader("Shaders\\spotLightPass.vert", "Shaders\\spotLightPass.frag");

	m_FullscrenQuad = new MeshNode();
	m_FullscrenQuad->loadModel("Models\\quad.obj");
	m_FullscrenQuad->setName("Fullscreen Quad");
	m_PointLightMesh = new MeshNode();
	m_PointLightMesh->loadModel("Models\\sphere.obj");
	m_PointLightMesh->setName("PointLight Sphere");
	m_SpotLightMesh = new MeshNode();
	m_SpotLightMesh->loadModel("Models\\sphere.obj");
	m_SpotLightMesh->setName("SpotLight Cone");

	m_DiffuseTexture = new TextureNode();
	m_DiffuseTexture->loadTexture("images/default.png");
	m_WhiteTexture = new TextureNode();
	m_WhiteTexture->loadTexture("images/white.png");
	m_BlackTexture = new TextureNode();
	m_BlackTexture->loadTexture("images/black.png");
	m_NormalTexture = new TextureNode();
	m_NormalTexture->loadTexture("images/defaultNormal.png");

	//m_StencilPassShader = new Shader();
	//m_StencilPassShader->initShader("shaders/stencilPass.vert", "shaders/stencilPass.frag");

	//m_DirLightShader = new Shader();
	//m_DirLightShader->initShader("shaders/dirLightPass.vert", "shaders/dirLightPass.frag");

	//m_PointLightShader = new Shader();
	//m_PointLightShader->initShader("shaders/pointLightPass.vert", "shaders/pointLightPass.frag");

	//enable standard opengl rendering features
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	SDL_GL_SetSwapInterval(0); // set swap buffers to sync with monitor's vertical refresh rate
	fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));

	//initialize the geometry buffer
	m_GBuffer = new GBuffer(m_ScreenWidth, m_ScreenHeight);
	m_GBuffer->init();

	m_CurrentShader = m_GeometryShader;
}
void DeferredRenderer::shutDown(void)
{
    SDL_GL_DeleteContext(m_Context);
    SDL_DestroyWindow(m_Window);
	m_Lights.clear();
	m_DataList.clear();
	m_CulledList.clear();
	m_PointLightList.clear();
	m_SpotLightList.clear();
	delete m_Frustum;
	m_Frustum = nullptr;
	delete m_DiffuseTexture;
	delete m_WhiteTexture;
	delete m_BlackTexture;
	delete m_NormalTexture;
	delete m_GeometryShader;
	delete m_UIShader;
	delete m_PointLightShader;
	delete m_SpotLightShader;
	delete m_CurrentShader;
	delete m_StencilPassShader;
	delete m_PointLightMesh;
	delete m_SpotLightMesh;
	delete m_DiffuseTexture;
	delete m_WhiteTexture;
	delete m_BlackTexture;
	delete m_NormalTexture;
}

void DeferredRenderer::geometryPass(std::vector<StandardDataSet> &p_DataList)
{
	m_GBuffer->initGeometryPass();	// Bind buffers

	glDepthMask(GL_TRUE);			// Make sure to turn on depth testing
	glEnable(GL_DEPTH_TEST);		// as this is much like a regular forward render pass

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Pick a shader, and render each data, looping through dataList vector
	for(unsigned int i=0; i < p_DataList.size(); i++)
	{
		int v_NumTextures = 0;
		p_DataList[i].SelectedShader->enable();

		//bind any textures, if they are present
		glActiveTexture(GL_TEXTURE0 + DIFFUSE);
		p_DataList[i].SelectedShader->bindTexture(DIFFUSE, p_DataList[i].DiffuseMapLocation);

		glActiveTexture(GL_TEXTURE0 + EMISSIVE);
		p_DataList[i].SelectedShader->bindTexture(EMISSIVE, p_DataList[i].EmissiveMapLocation);

		glActiveTexture(GL_TEXTURE0 + NORMAL);
		p_DataList[i].SelectedShader->bindTexture(NORMAL, p_DataList[i].NormalMapLocation);

		/*if(v_NumTextures > HEIGHT)
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D,p_DataList[i].DepthMapLocation);
			int textureLocation = glGetUniformLocation(p_DataList[i].SelectedShader->getShaderLocation(),"heightMap");
			glUniform1i(textureLocation,HEIGHT);
		}*/

		glBindVertexArray(p_DataList[i].MeshLocation);

		p_DataList[i].SelectedShader->setModelMatrix(p_DataList[i].ModelMatrix);
		p_DataList[i].SelectedShader->setProjectionMatrix(p_DataList[i].projectionMatrix);
		p_DataList[i].SelectedShader->setModelView(p_DataList[i].viewMatrix * p_DataList[i].ModelMatrix);

		glDrawElements(GL_TRIANGLES, p_DataList[i].MeshVertCount, GL_UNSIGNED_INT, 0);	// draw VAO 

		glBindVertexArray(0);
		//disable shader
		p_DataList[i].SelectedShader->disable();
	}

	//glDepthMask(GL_FALSE);
}
void DeferredRenderer::UIPass(std::vector<UIDataSet> &p_DataList)
{
	m_GBuffer->bindForWriting(GBuffer::GBufferFinal);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);			// turn off depth testing as the UI is drawn over everything else
	glDisable(GL_DEPTH_TEST);
	
	// Pick a shader, and render each data, looping through dataList vector
	for(std::vector<UIDataSet>::size_type i = 0; i < p_DataList.size(); i++)
	{
		p_DataList[i].SelectedShader->enable();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,p_DataList[i].DiffuseMapLocation);
		
		glBindVertexArray(p_DataList[i].MeshLocation);
		p_DataList[i].SelectedShader->setModelMatrix(p_DataList[i].ModelMatrix);
		p_DataList[i].SelectedShader->setProjectionMatrix(p_DataList[i].projectionMatrix);
		p_DataList[i].SelectedShader->setModelView(p_DataList[i].viewMatrix * p_DataList[i].ModelMatrix);
		glDrawElements(GL_TRIANGLES, p_DataList[i].MeshVertCount, GL_UNSIGNED_INT, 0);	// draw VAO 
	}
	glBindVertexArray(0);
	glDisable(GL_BLEND);
}

void DeferredRenderer::stencilPass(glm::mat4 &p_lightModelMat, MeshNode &p_lightMesh)
{
	m_GBuffer->initStencilPass();	// Unbind any buffers

	m_StencilPassShader->enable();
	m_StencilPassShader->setModelMatrix(m_ProjectionMatrix * m_CurrentViewMatrix * p_lightModelMat);

	glEnable(GL_DEPTH_TEST);		// Depth test is a primary operation of stencil pass
    glDisable(GL_CULL_FACE);		// We want both faces to be rendered, so it can test if a fragment is inside an object

	glClear(GL_STENCIL_BUFFER_BIT);		// Clear stencil buffer
	glStencilFunc(GL_ALWAYS, 0, 0);		// and set it to always pass the test (as we are generating it in this pass, not using it to minimize fragment renders)

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);		// If a fragment is closer than the back face of the model, increment stencil value
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);		// If a fragment is close than the front face, decrement the stencil value
																		// So, if a fragment is inside a model, it's stencil value is increased and then kept increased (stencil + 1 - 0 = stencil++)
																		// If a fragment is in front of the model, increase it's stencil value and then decrease it		(stencil + 1 - 1 = stencil)
																		// If a fragmant is behind the model, just keep it's stencil value								(stencil + 0 - 0 = stencil)
	
	glBindVertexArray(p_lightMesh.getMeshLocation());
	glDrawElements(GL_TRIANGLES, p_lightMesh.getNumVerts(), GL_UNSIGNED_INT, 0);	// draw VAO 
	//glBindVertexArray(0);
}
void DeferredRenderer::dirLightPass()
{
	glDisable(GL_DEPTH_TEST);		
	glEnable(GL_BLEND);				// Enable blending, so the default state of fragments are dark (clearing final buffer to black)
	glBlendEquation(GL_FUNC_ADD);	// unless they get lit by directional (or anyother) light
	glBlendFunc(GL_ONE, GL_ONE);	

	//TODO: set directional light data uniforms and render a quad using directional light shader
	
	glDisable(GL_BLEND);
}
void DeferredRenderer::pointLightPass(PointLightDataSet &p_lightData)
{
	m_GBuffer->initLightPass();

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);	// Set stencil test to only render if a stencil value is greater than 0 (zero)

	glDisable(GL_DEPTH_TEST);		
	glEnable(GL_BLEND);				// Use blending so that unlit fragments are black (final buffer is cleared to black every frame)
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
        
	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);
    glCullFace(GL_FRONT);	// Use face culling, so that a fragment doesn't receive light (and light calculations) for both faces
							// Cull the front face (and not the back) so that the light will still be calculated even if the camera is inside the sphere

	// TODO: set point light's uniform values, calculate sphere size (based on radius) and render a sphere using point light shader

	//p_lightData.lightModel = glm::scale(p_lightData.lightModel, p_lightData.calcLightScale());

	m_PointLightShader->enable();
	//glm::mat4 currentModel = p_lightData.lightModel * glm::vec4(p_lightData.worldPosition,1.0f);

	m_PointLightShader->setModelMatrix(m_ProjectionMatrix * m_CurrentViewMatrix * p_lightData.lightModel);
	m_PointLightShader->setWorldPosition(p_lightData.worldPosition);
	m_PointLightShader->setColour(p_lightData.colour);
	m_PointLightShader->setAttenConstant(p_lightData.attenuation.x);
	m_PointLightShader->setAttenLinear(p_lightData.attenuation.y);
	m_PointLightShader->setAttenQuadratic(p_lightData.attenuation.z);
	m_PointLightShader->setAmbientIntensity(p_lightData.ambientI);
	m_PointLightShader->setDiffuseIntensity(p_lightData.diffuseI);
	m_PointLightShader->setSpecularIntensity(0.1f);//p_lightData.SpecI);
	m_PointLightShader->setSpecularPower(8.0f);//p_lightData.SpecP);
	m_PointLightShader->setCameraWorldPosition(m_CameraPosition);
	//m_PointLightShader->setCameraWorldPosition(glm::vec3(m_CurrentViewMatrix[3].x, m_CurrentViewMatrix[3].y, m_CurrentViewMatrix[3].z));
	//m_PointLightShader->setCameraWorldPosition(glm::vec3(m_CurrentViewMatrix*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	m_PointLightShader->setScreenSize(glm::vec2((float)m_ScreenWidth, (float)m_ScreenHeight));
	
	glm::vec3 view = glm::vec3(m_CurrentViewMatrix*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	m_PointLightShader->bindPositionBuffer(m_GBuffer->getPositionBufferHandle());
	m_PointLightShader->bindDiffuseBuffer(m_GBuffer->getDiffuseBufferHandle());
	m_PointLightShader->bindNormalBuffer(m_GBuffer->getNormalBufferHandle());

	glBindVertexArray(m_PointLightMesh->getMeshLocation());
	glDrawElements(GL_TRIANGLES, m_PointLightMesh->getNumVerts(), GL_UNSIGNED_INT, 0);	// draw VAO 
	//glDrawElementsBaseVertex(GL_TRIANGLES, m_PointLightMesh->getNumVerts(), GL_UNSIGNED_INT, 0, 0);
	//glBindVertexArray(0);

    glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}
void DeferredRenderer::spotLightPass(SpotLightDataSet &p_lightData)
{
	m_GBuffer->initLightPass();
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

	//p_lightData.lightModel = glm::scale(p_lightData.lightModel, p_lightData.calcLightScale());

	m_SpotLightShader->enable();

	m_SpotLightShader->setModelMatrix(m_ProjectionMatrix * m_CurrentViewMatrix * p_lightData.lightModel);
	m_SpotLightShader->setWorldPosition(p_lightData.worldPosition);
	m_SpotLightShader->setWorldDirection(glm::normalize(p_lightData.worldDirection));
	//m_SpotLightShader->setWorldDirection(glm::vec3(1.0f, 1.0f, 1.0f));
	m_SpotLightShader->setColour(p_lightData.colour);
	m_SpotLightShader->setAttenConstant(p_lightData.attenuation.x);
	m_SpotLightShader->setAttenLinear(p_lightData.attenuation.y);
	m_SpotLightShader->setAttenQuadratic(p_lightData.attenuation.z);
	m_SpotLightShader->setAmbientIntensity(p_lightData.ambientI);
	m_SpotLightShader->setDiffuseIntensity(p_lightData.diffuseI);
	m_SpotLightShader->setSpecularIntensity(p_lightData.SpecI);
	m_SpotLightShader->setSpecularPower(p_lightData.SpecP);
	m_SpotLightShader->setCutoffAngle(p_lightData.cutoffAngle * (float)PI / 180.0f);
	m_SpotLightShader->setCameraWorldPosition(m_CameraPosition);
	m_SpotLightShader->setScreenSize(glm::vec2((float)m_ScreenWidth, (float)m_ScreenHeight));
	
	m_SpotLightShader->bindPositionBuffer(m_GBuffer->getPositionBufferHandle());
	m_SpotLightShader->bindDiffuseBuffer(m_GBuffer->getDiffuseBufferHandle());
	m_SpotLightShader->bindNormalBuffer(m_GBuffer->getNormalBufferHandle());

	glBindVertexArray(m_PointLightMesh->getMeshLocation());
	glDrawElements(GL_TRIANGLES, m_PointLightMesh->getNumVerts(), GL_UNSIGNED_INT, 0);	// draw VAO 
	//glBindVertexArray(0);
}
void DeferredRenderer::skyboxPass()
{
	glEnable(GL_DEPTH_TEST);	// Enable depth test
								// and make sure that the skybox is as big as Z FAR value, so it only get's rendered to the empty areas behind ALL the objects
	// Render skybox here
}
void DeferredRenderer::finalPass()
{
	m_GBuffer->initFinalPass();																	// Bind required framebuffers
																								// Copy a buffer to another buffer
	glBlitFramebuffer(	0, 0, m_ScreenWidth, m_ScreenHeight,									// Copy an area from origini to screen size (full screen area)
						0, 0, m_ScreenWidth, m_ScreenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);	// and paste it to the same sized full screen area, using 'NEAREST' filter,
}																								// to avoid any artifacts, since the buffers are same size (pixels are 1 to 1)
void DeferredRenderer::blurPass(GBuffer::GBufferTextureType p_sourceBuffer, GBuffer::GBufferTextureType p_destinationBuffer, int p_numPasses, float p_initialBlurOffset, float p_progressiveBlurOffset)
{
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_ONE, GL_ZERO);
	//glBlendFunc(GL_ONE, GL_ONE);
	float currentBlurOffset = p_initialBlurOffset;
	m_BlurHorizontalShader->enable();
	m_BlurHorizontalShader->setBlurOffset(currentBlurOffset);
	m_GBuffer->bindForReading(p_sourceBuffer);
	m_GBuffer->bindForWriting(GBuffer::GBufferBlur);

	glBindVertexArray(m_FullscrenQuad->getMeshLocation());
	glDrawElements(GL_TRIANGLES, m_FullscrenQuad->getNumVerts(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	for(int i = 1; i < p_numPasses; i++)
	{
		m_GBuffer->bindForReading(GBuffer::GBufferBlur);
		m_GBuffer->bindForWriting(p_sourceBuffer);
		m_BlurVerticalShader->enable();
		m_BlurVerticalShader->setBlurOffset(currentBlurOffset);

		glBindVertexArray(m_FullscrenQuad->getMeshLocation());
		glDrawElements(GL_TRIANGLES, m_FullscrenQuad->getNumVerts(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		currentBlurOffset += p_progressiveBlurOffset;
		m_GBuffer->bindForReading(p_sourceBuffer);
		m_GBuffer->bindForWriting(GBuffer::GBufferBlur);
		m_BlurHorizontalShader->enable();
		m_BlurHorizontalShader->setBlurOffset(currentBlurOffset);

		glBindVertexArray(m_FullscrenQuad->getMeshLocation());
		glDrawElements(GL_TRIANGLES, m_FullscrenQuad->getNumVerts(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	glBlendFunc(GL_ONE, GL_ONE);
	m_BlurVerticalShader->enable();
	m_BlurVerticalShader->setBlurOffset(currentBlurOffset);
	m_GBuffer->bindForReading(GBuffer::GBufferBlur);
	m_GBuffer->bindForWriting(p_destinationBuffer);

	glBindVertexArray(m_FullscrenQuad->getMeshLocation());
	glDrawElements(GL_TRIANGLES, m_FullscrenQuad->getNumVerts(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

DeferredRenderer::~DeferredRenderer(void)
{
	m_Window = nullptr;
}

void DeferredRenderer::cullDataSet(std::vector<StandardDataSet> p_Unculled, std::vector<StandardDataSet> p_Culled, glm::mat4& p_ViewMatrix)
{int count =0; 
	for(std::vector<StandardDataSet>::iterator it = p_Unculled.begin(); it != p_Unculled.end(); it++)
	{		
		if( m_Frustum->frustumCheck( (*it).modelPosition, (*it).boundingRadius) )
			{
					m_CulledList.push_back((*it));
					count++; 
		}
	}//std::cout << "objects rendered:" << count << std::endl;
}