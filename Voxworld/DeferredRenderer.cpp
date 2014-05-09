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
#include "ParticleSystem.h"


DeferredRenderer::DeferredRenderer(int p_WindowWidth, int p_WindowHeight):Renderer(p_WindowWidth, p_WindowHeight)
{
		m_UI_Phase = false;
		m_Frustum = new Frustum();
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
				m_GBuffer->setFinalPassBuffer(GBuffer::GBufferDiffuse);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_CULL_FACE);
				break;
			}
		case(FILLED):
			{
				m_GBuffer->setFinalPassBuffer(GBuffer::GBufferFinal);
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

		if(m_CurrentSpecularMap != 0)
			v_DataSet.SpecularLocation = m_CurrentSpecularMap;
		else
			v_DataSet.SpecularLocation = m_BlackTexture->getTexture();

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
	m_CurrentSpecularMap = 0;
}
//this is called only when every object to be rendered has called its own render function
void DeferredRenderer::endRenderCycle(void)
{
	m_GBuffer->initFrame();		// Required to clear final buffer

	cullDataSet(m_DataList, m_CulledList, m_CurrentViewMatrix);

	geometryPass(m_CulledList);	// Render geometry of objects, pass vector of dataSets for current frame
	
	lightPass();

	particlesPass(m_ParticlesList);

	emissiveToFinalPass();

	blurPass(GBuffer::GBufferEmissive, GBuffer::GBufferFinal, 3, 1.0f, -0.2f);

	UIPass(m_UIDataList);	// Render UI objects, pass vector of UI dataSets for current frame

	finalPass();

	// Clear current frame data sets, to get ready for the next frame
	m_DataList.clear();			
	m_CulledList.clear();
	m_UIDataList.clear();
	m_PointLightList.clear();
	m_SpotLightList.clear();
	m_ParticlesList.clear();

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
		case(SPECULAR):
		{
			m_CurrentSpecularMap = p_TextureNode->getTexture();
			m_Textures[SPECULAR] = true;
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
		glm::vec3(p_PointLightNode->getWorldTransform()*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),		p_PointLightNode->getColour(),				
		p_PointLightNode->getAttenuation(),			p_PointLightNode->getAmbientIntensity(),	p_PointLightNode->getDiffuseIntensity() ));
}
void DeferredRenderer::render(SpotLight* p_SpotLightNode)
{
	m_SpotLightList.push_back(SpotLightDataSet(
		p_SpotLightNode->getTransformedPosition(),	p_SpotLightNode->getColour(),	p_SpotLightNode->getAttenuation(),
		glm::normalize(p_SpotLightNode->getTransformedDirection()),					cosf(p_SpotLightNode->getCutoffAngle() * (float)PI / 180.0f),
		p_SpotLightNode->getAmbientIntensity(),										p_SpotLightNode->getDiffuseIntensity()	));
}
void DeferredRenderer::render(ParticleSystem* p_Particle)
{
	m_ParticlesList.push_back(ParticleDataSet(	p_Particle->getParticleHandle(), p_Particle->getNumParticles(), p_Particle->getBoundingRadius(), 
												m_ProjectionMatrix * m_CurrentViewMatrix * m_CurrentModelMatrix, p_Particle->getCurrentColour(), 
												p_Particle->getPointSize(), p_Particle->getMaxLifeTime()	));
}

void DeferredRenderer::toggleFullscreen()
{
	if(m_Fullscreen)
	{
		SDL_SetWindowFullscreen(m_Window, SDL_FALSE);
		resizeWindow(m_WindowedScrenWidth, m_WindowedScreenHeight);
	}
	else
	{
		resizeWindow(m_FullscreenWidth, m_FullscreenHeight);
		SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	m_Fullscreen = !m_Fullscreen;
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
        m_CurrentScreenWidth, m_CurrentScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
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

	// Get current display settings and set fullscreen resolution as maximum screen resolution
	SDL_DisplayMode currentDisplayMode;
	SDL_GetCurrentDisplayMode(0, &currentDisplayMode);
	m_FullscreenHeight = currentDisplayMode.h;
	m_FullscreenWidth = currentDisplayMode.w;

	m_ProjectionMatrix = glm::perspective(m_FOV, (float)m_CurrentScreenWidth / (float)m_CurrentScreenHeight, m_zNear, m_zFar);

	//repeat next 4 lines for each additional shader
	m_CurrentShader = nullptr;
	m_GeometryShader = new Shader();
	m_GeometryShader->initShader("Shaders\\geometryPass.vert","Shaders\\geometryPass.frag");
	m_UIShader = new Shader();
	m_UIShader->initShader("Shaders\\UIPass.vert","Shaders\\UIPass.frag");
	
	m_emissiveToFinalShader = new LightShader();
	m_emissiveToFinalShader->initShader("Shaders\\emissivePassThrough.vert","Shaders\\emissivePassThrough.frag");
	m_lightPassShader = new LightShader();
	m_lightPassShader->initShader("Shaders\\lightPass.vert", "Shaders\\lightPass.frag");
	m_ParticleShader = new ParticleShader();
	m_ParticleShader->initShader("Shaders\\particlesEffect.vert", "Shaders\\particlesEffect.frag");
	m_BlurVerticalShader = new GaussianBlurShader();
	m_BlurVerticalShader->initShader("Shaders\\gaussianBlurVertical.vert", "Shaders\\gaussianBlurVertical.frag");
	m_BlurHorizontalShader = new GaussianBlurShader();
	m_BlurHorizontalShader->initShader("Shaders\\gaussianBlurHorizontal.vert", "Shaders\\gaussianBlurHorizontal.frag");

	m_FullscrenQuad = new MeshNode();
	m_FullscrenQuad->loadModel("Models\\quad.obj");
	m_FullscrenQuad->setName("Fullscreen Quad");

	m_DiffuseTexture = new TextureNode();
	m_DiffuseTexture->loadTexture("images/default.png");
	m_WhiteTexture = new TextureNode();
	m_WhiteTexture->loadTexture("images/white.png");
	m_BlackTexture = new TextureNode();
	m_BlackTexture->loadTexture("images/black.png");
	m_NormalTexture = new TextureNode();
	m_NormalTexture->loadTexture("images/defaultNormal.png");
	
	m_maxNumPointLights = m_lightPassShader->getPointLightBlockSize() / sizeof(PointLightDataSet);
	m_maxNumSpotLights = m_lightPassShader->getSpotLightBlockSize() / sizeof(SpotLightDataSet);
	
	glGenBuffers(1, &m_PointLightBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_PointLightBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLightDataSet) * m_maxNumPointLights, NULL, GL_DYNAMIC_DRAW);
	//glBindBufferBase(GL_UNIFORM_BUFFER, m_lightPassShader->getPointLightBlockIndex(), m_PointLightBuffer);
	
	glGenBuffers(1, &m_SpotLightBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_SpotLightBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SpotLightDataSet) * m_maxNumSpotLights, NULL, GL_DYNAMIC_DRAW);
	//glBindBufferBase(GL_UNIFORM_BUFFER, m_lightPassShader->getSpotLightBlockIndex(), m_SpotLightBuffer);

	//enable standard opengl rendering features
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	SDL_GL_SetSwapInterval(0); // set swap buffers to sync with monitor's vertical refresh rate
	fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));

	//initialize the geometry buffer
	m_GBuffer = new GBuffer(m_CurrentScreenWidth, m_CurrentScreenHeight);
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
	delete m_CurrentShader;
	delete m_lightPassShader;
	delete m_DiffuseTexture;
	delete m_WhiteTexture;
	delete m_BlackTexture;
	delete m_NormalTexture;
	delete m_emissiveToFinalShader;
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

		glActiveTexture(GL_TEXTURE0 + SPECULAR);
		p_DataList[i].SelectedShader->bindTexture(SPECULAR, p_DataList[i].SpecularLocation);

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
		p_DataList[i].SelectedShader->setCameraWorldPosition(m_CameraPosition);

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
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);			// turn off depth testing as the UI is drawn over everything else
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	
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

void DeferredRenderer::lightPass()
{
	m_GBuffer->initLightPass();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glBindBuffer(GL_UNIFORM_BUFFER, m_PointLightBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PointLightDataSet) * m_PointLightList.size(), m_PointLightList.data());
	
	glBindBuffer(GL_UNIFORM_BUFFER, m_SpotLightBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SpotLightDataSet) * m_SpotLightList.size(), m_SpotLightList.data());

	m_lightPassShader->enable();
	m_lightPassShader->setScreenSize(glm::vec2((float)m_CurrentScreenWidth, (float)m_CurrentScreenHeight));
	m_lightPassShader->setNumPointLights(m_PointLightList.size());
	m_lightPassShader->setNumSpotLights(m_SpotLightList.size());
	m_lightPassShader->setCameraWorldPosition(m_CameraPosition);

	m_lightPassShader->bindPositionBuffer(m_GBuffer->getPositionBufferHandle());
	m_lightPassShader->bindDiffuseBuffer(m_GBuffer->getDiffuseBufferHandle());
	m_lightPassShader->bindNormalBuffer(m_GBuffer->getNormalBufferHandle());
	//m_lightPassShader->bindEmissiveBuffer(m_GBuffer->getEmissiveBufferHandle());
	
	m_lightPassShader->bindPointLightBuffer(m_PointLightBuffer);
	m_lightPassShader->bindSpotLightBuffer(m_SpotLightBuffer);

	glBindVertexArray(m_FullscrenQuad->getMeshLocation());
	glDrawElements(GL_TRIANGLES, m_FullscrenQuad->getNumVerts(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void DeferredRenderer::emissiveToFinalPass()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	
	//m_GBuffer->initLightPass();

	m_GBuffer->bindForReading(GBuffer::GBufferPosition, 0);
	m_GBuffer->bindForReading(GBuffer::GBufferEmissive, 1);

	m_GBuffer->bindForWriting(GBuffer::GBufferFinal);

	m_emissiveToFinalShader->enable();
	m_emissiveToFinalShader->setCameraWorldPosition(m_CameraPosition);
	m_emissiveToFinalShader->setScreenSize(glm::vec2((float)m_CurrentScreenWidth, (float)m_CurrentScreenHeight));
	
	m_emissiveToFinalShader->bindPositionBuffer(0);
	m_emissiveToFinalShader->bindEmissiveBuffer(1);
	
	glBindVertexArray(m_FullscrenQuad->getMeshLocation());
	glDrawElements(GL_TRIANGLES, m_FullscrenQuad->getNumVerts(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void DeferredRenderer::finalPass()
{
	m_GBuffer->initFinalPass();																	// Bind required framebuffers
																								// Copy a buffer to another buffer
	glBlitFramebuffer(	0, 0, m_CurrentScreenWidth, m_CurrentScreenHeight,									// Copy an area from origini to screen size (full screen area)
						0, 0, m_CurrentScreenWidth, m_CurrentScreenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);	// and paste it to the same sized full screen area, using 'NEAREST' filter,
}																								// to avoid any artifacts, since the buffers are same size (pixels are 1 to 1)
void DeferredRenderer::particlesPass(std::vector<ParticleDataSet> &p_particleList)
{
	m_GBuffer->initParticlePass();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	//glBlendFunc(GL_ONE, GL_ZERO);
	glBlendFunc(GL_ONE, GL_ONE);

	m_ParticleShader->enable();
	glEnable(GL_POINTS);
	//m_GBuffer->bindForWriting(GBuffer::GBufferFinal);
	//m_GBuffer->bindForWriting(GBuffer::GBufferEmissive);
	m_ParticleShader->bindEmissiveBuffer(GBuffer::GBufferEmissive);
	m_ParticleShader->bindFinalBuffer(GBuffer::GBufferEmissive);
	for(std::vector<ParticleDataSet>::size_type i = 0, numParticles = p_particleList.size(); i < numParticles; i++)
	{
		m_ParticleShader->setColour(p_particleList[i].colour);
		m_ParticleShader->setMVP(p_particleList[i].MVP);
		m_ParticleShader->setMaxLifeTime(p_particleList[i].maxLifeTime);

		glBindVertexArray(p_particleList[i].VAO);
		glPointSize(p_particleList[i].pointSize);
		glDrawArrays(GL_POINTS, 0, p_particleList[i].numParticles);
	}
	glBindVertexArray(0);
}
void DeferredRenderer::blurPass(GBuffer::GBufferTextureType p_sourceBuffer, GBuffer::GBufferTextureType p_destinationBuffer, int p_numPasses, float p_initialBlurOffset, float p_progressiveBlurOffset)
{
	glDisable(GL_DEPTH_TEST);
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
void DeferredRenderer::resizeWindow(int p_width, int p_height)
{
	m_CurrentScreenWidth = p_width;
	m_CurrentScreenHeight = p_height;
	m_GBuffer->reload(m_CurrentScreenWidth, m_CurrentScreenHeight);
	m_ProjectionMatrix = glm::perspective(m_FOV, (float)m_CurrentScreenWidth / (float)m_CurrentScreenHeight, m_zNear, m_zFar);
	glViewport(0, 0, m_CurrentScreenWidth, m_CurrentScreenHeight);
	SDL_SetWindowSize(m_Window, m_CurrentScreenWidth, m_CurrentScreenHeight);
}