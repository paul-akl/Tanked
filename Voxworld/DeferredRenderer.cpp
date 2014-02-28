#include "DeferredRenderer.h"
#include "TextureNode.h"
#include "TransformNode.h"
#include "MeshNode.h"
#include "CameraNode.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GeneralUtils.h"
#include "Shader.h"

DeferredRenderer::DeferredRenderer(int p_WindowWidth, int p_WindowHeight):Renderer(p_WindowWidth, p_WindowHeight)
{
		m_UI_Phase = false;
}

void DeferredRenderer::begin(void)
{
}
void DeferredRenderer::render(LightNode* p_LightNode)
{
	m_Lights.push_back(p_LightNode->getData());
}
void DeferredRenderer::beginRenderCycle(RenderMode p_Mode)
{
	// bind the GBuffer to write to
	m_GBuffer->enable();
	//clear the buffer with specified colour
	glClearColor(0.0f,0.0f,0.0f,1.0f);
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
	m_ProjectionMatrix = glm::perspective(60.0f, (float)m_ScreenWidth / (float)m_ScreenHeight, 1.0f, 200.0f);
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

	for(size_t i = 0; i < 4; i++)
	{
		switch(m_Textures[i])
		{
		case(true):
			{
				v_NumTextures++;
				switch(i)
				{
				case(DIFFUSE):
					{
						m_CurrentShader = m_Shaders[DIFFUSE];
						break;
					}
				case(NORMAL):
					{
						m_CurrentShader = m_Shaders[NORMAL];
						break;
					}
				case(SPECULAR):
					{
						m_CurrentShader = m_Shaders[SPECULAR];
						break;
					}
				case(DEPTH):
					{
						m_CurrentShader = m_Shaders[DEPTH];
						break;
					}
				}
			}
		case(false):
			{
				break;
			}
		}
	}

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
		v_DataSet.DiffuseMapLocation = m_CurrentDiffuse;
		v_DataSet.NormalMapLocation = m_CurrentNormalMap;
		v_DataSet.SpecularMapLocation = m_CurrentSpecMap;
		v_DataSet.DepthMapLocation = m_CurrentDepthMap;
		//material
		v_DataSet.Material = m_CurrentMaterial;
		//shader
		v_DataSet.SelectedShader = m_CurrentShader;
		//non camera related transform matrices
		v_DataSet.ModelMatrix = m_CurrentModelMatrix;
		v_DataSet.viewMatrix = m_CurrentViewMatrix;
		v_DataSet.normalMatrix = m_NormalMatrix;
		v_DataSet.projectionMatrix = m_ProjectionMatrix;

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
		v_UIDataSet.SelectedShader = m_CurrentShader;
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
}
//this is called only when every object to be rendered has called its own render function
void DeferredRenderer::endRenderCycle(void)
{
	m_GBuffer->initFrame();		// Required to clear final buffer
	//somewhere here will be a frustum check
	geometryPass(m_DataList);	// Render geometry of objects, pass vector of dataSets for current frame
	geometryPass(m_UIDataList);	// Render geometry of UI objects, pass vector of UI dataSets for current frame
	
	finalPass();

	m_DataList.clear();			// Clear current frame data sets, to get ready for the next frame
	m_UIDataList.clear();

	m_Matrices[VIEW] = false;
	m_Matrices[PROJECTION] = false;
	m_UI_Phase = false;

	SDL_GL_SwapWindow(m_Window); // swap buffers
}
void DeferredRenderer::beginUIPhase(void)
{
	m_UI_Phase = true;
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
		case(SPECULAR):
		{
			m_CurrentSpecMap = p_TextureNode->getTexture();
			m_Textures[SPECULAR] = true;
			break;
		}
		case(DEPTH):
		{
			m_CurrentDepthMap = p_TextureNode->getTexture();
			m_Textures[DEPTH] = true;
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
	m_Matrices[VIEW] = true;
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

	//repeat next 4 lines for each additional shader
	m_CurrentShader = new Shader();
	m_CurrentShader->initShader("phong-tex.vert","phong-tex.frag");
	m_Shaders.push_back(m_CurrentShader);
	m_CurrentShader = nullptr;

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

	SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate
	fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));

	//initialize the geometry buffer
	m_GBuffer = new GBuffer(m_ScreenWidth, m_ScreenHeight);
	m_GBuffer->init();
}
void DeferredRenderer::shutDown(void)
{
    SDL_GL_DeleteContext(m_Context);
    SDL_DestroyWindow(m_Window);
	m_Shaders.clear();
	m_Lights.clear();
	m_DataList.clear();
}

void DeferredRenderer::geometryPass(std::vector<StandardDataSet> &p_DataList)
{
	m_GBuffer->initGeometryPass();	// Bind buffers

	glDepthMask(GL_TRUE);			// Make sure to turn on depth testing
	glEnable(GL_DEPTH_TEST);		// as this is much like a regular forward render pass

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Pick a shader, and render each data, looping through dataList vector
	for(int i=0; i < p_DataList.size(); i++)
	{
		int v_NumTextures = 0;
		p_DataList[i].SelectedShader->enable();

		//bind any textures, if they are present
		if(v_NumTextures >= DIFFUSE)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,p_DataList[i].DiffuseMapLocation);
			GLint shaderLocation = p_DataList[i].SelectedShader->getShaderLocation();
			GLint textureLocation = glGetUniformLocation(shaderLocation,"diffuseMap");
			glUniform1i(textureLocation,DIFFUSE);
		}
		if(v_NumTextures > SPECULAR)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D,p_DataList[i].SpecularMapLocation);
			int textureLocation = glGetUniformLocation(p_DataList[i].SelectedShader->getShaderLocation(),"specularMap");
			glUniform1i(textureLocation,SPECULAR);
		}
		if(v_NumTextures > NORMAL)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D,p_DataList[i].NormalMapLocation);
			int textureLocation = glGetUniformLocation(p_DataList[i].SelectedShader->getShaderLocation(),"normalMap");
			glUniform1i(textureLocation,NORMAL);
		}
		if(v_NumTextures > DEPTH)
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D,p_DataList[i].DepthMapLocation);
			int textureLocation = glGetUniformLocation(p_DataList[i].SelectedShader->getShaderLocation(),"depthMap");
			glUniform1i(textureLocation,DEPTH);
		}

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

void DeferredRenderer::geometryPass(std::vector<UIDataSet> &p_DataList)
{
	m_GBuffer->initGeometryPass();	// Bind buffers

	glDepthMask(GL_FALSE);			// turn off depth testing as the UI is drawn over everything else
	glDisable(GL_DEPTH_TEST);		
	//glDisable(GL_CULL_FACE);
	//glClear(GL_DEPTH_BUFFER_BIT);
	
	// Pick a shader, and render each data, looping through dataList vector
	for(int i=0; i < p_DataList.size(); i++)
	{
		int v_NumTextures = 0;
		p_DataList[i].SelectedShader->enable();

		//bind any textures, if they are present
		if(v_NumTextures >= DIFFUSE)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,p_DataList[i].DiffuseMapLocation);
			GLint shaderLocation = p_DataList[i].SelectedShader->getShaderLocation();
			GLint textureLocation = glGetUniformLocation(shaderLocation,"diffuseMap");
			glUniform1i(textureLocation,DIFFUSE);
		}
		
		glBindVertexArray(p_DataList[i].MeshLocation);

		p_DataList[i].SelectedShader->setModelMatrix(p_DataList[i].ModelMatrix);
		p_DataList[i].SelectedShader->setProjectionMatrix(p_DataList[i].projectionMatrix);
		p_DataList[i].SelectedShader->setModelView(p_DataList[i].viewMatrix * p_DataList[i].ModelMatrix);

		glDrawElements(GL_TRIANGLES, p_DataList[i].MeshVertCount, GL_UNSIGNED_INT, 0);	// draw VAO 

		glBindVertexArray(0);
		//disable shader
		p_DataList[i].SelectedShader->disable();
	}

	glDepthMask(GL_TRUE);			// Make sure to turn on depth testing
	glEnable(GL_DEPTH_TEST);		// as this is much like a regular forward render pass
}

void DeferredRenderer::stencilPass()
{
	m_GBuffer->initStencilPass();	// Unbind any buffers

	glEnable(GL_DEPTH_TEST);		// Depth test is a primary operation of stencil pass
    glDisable(GL_CULL_FACE);		// We want both faces to be rendered, so it can test if a fragment is inside an object

	glClear(GL_STENCIL_BUFFER_BIT);		// Clear stencil buffer
	glStencilFunc(GL_ALWAYS, 0, 0);		// and set it to always pass the test (as we are generating it in this pass, not using it to minimize fragment renders)

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);		// If a fragment is closer than the back face of the model, increment stencil value
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);		// If a fragment is close than the front face, decrement the stencil value
																		// So, if a fragment is inside a model, it's stencil value is increased and then kept increased (stencil + 1 - 0 = stencil++)
																		// If a fragment is in front of the model, increase it's stencil value and then decrease it		(stencil + 1 - 1 = stencil)
																		// If a fragmant is behind the model, just keep it's stencil value								(stencil + 0 - 0 = stencil)

	//TODO: set light's position, calculate it's radius and render a sphere using stencil shaders
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
void DeferredRenderer::pointLightPass()
{
	m_GBuffer->initLightPass();

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);	// Set stencil test to only render if a stencil value is greater than 0 (zero)

	glDisable(GL_DEPTH_TEST);		
	glEnable(GL_BLEND);				// Use blending so that unlit fragments are black (final buffer is cleared to black every frame)
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
        
    glCullFace(GL_FRONT);	// Use face culling, so that a fragment doesn't receive light (and light calculations) for both faces
							// Cull the front face (and not the back) so that the light will still be calculated even if the camera is inside the sphere

	// TODO: set point light's uniform values, calculate sphere size (based on radius) and render a sphere using point light shader
	
    glCullFace(GL_BACK);

	glDisable(GL_BLEND);
}
void DeferredRenderer::spotLightPass()
{
	// Not implemented yet
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

DeferredRenderer::~DeferredRenderer(void)
{
	m_Window = nullptr;
}
