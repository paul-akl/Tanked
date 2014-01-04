#include "GLTestRender.h"
#include "TextureNode.h"
#include "TransformNode.h"
#include "MeshNode.h"
#include "CameraNode.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GeneralUtils.h"

GLTestRender::GLTestRender(int p_WindowWidth, int p_WindowHeight):Renderer(p_WindowWidth, p_WindowHeight)
{

}
void GLTestRender::begin(void)
{

}
void GLTestRender::render(LightNode* p_LightNode)
{
	m_Lights.push_back(p_LightNode->getData());
}
void GLTestRender::beginRenderCycle(RenderMode p_Mode)
{

	//clear the back buffer with specified colour
	glClearColor(1.0f,1.0f,0.0f,1.0f);
	//fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));

	//clear colour buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));

	m_CurrentMode = p_Mode;
	//reset all flags for new rendering cycle
	for (size_t i = 0;i<4;i++)
	{
		m_Matrices[i] = false;
		m_Textures[i] = false;
	}
	m_ProjectionMatrix = glm::perspective(60.0f,m_ScreenWidth/m_ScreenHeight,1.0f,100.0f);
	m_Matrices[PROJECTION] = true;
}
void GLTestRender::end(void)
{
	glm::mat3 NormalMatrix = glm::inverse(glm::transpose(glm::mat3(m_CurrentViewMatrix*m_CurrentModelMatrix)));
	m_Matrices[NORMALMATRIX] = true;
	glm::mat4 modelview = m_CurrentViewMatrix*m_CurrentModelMatrix;
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

	//enable selected shader, and set uniforms
	m_CurrentShader->enable();
	if(m_Matrices[PROJECTION])
		m_CurrentShader->setProjectionMatrix(m_ProjectionMatrix);
	if(m_Matrices[MODEL])
		if(m_Matrices[VIEW])
			m_CurrentShader->setModelView(modelview);
	if(m_Matrices[NORMALMATRIX])	
		m_CurrentShader->setNormalMatrix(NormalMatrix);

	//fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));
	//bind any textures, if they are present
	if(v_NumTextures >= DIFFUSE)
	{
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,m_CurrentDiffuse);
		GLint shaderLocation = m_CurrentShader->getShaderLocation();
		GLint textureLocation = glGetUniformLocation(shaderLocation,"diffuseMap");
		glUniform1i(textureLocation,DIFFUSE);
		//fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));
	}
	if(v_NumTextures > SPECULAR)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,m_CurrentSpecMap);
		int textureLocation = glGetUniformLocation(m_CurrentShader->getShaderLocation(),"specularMap");
		glUniform1i(textureLocation,SPECULAR);
		//fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));
	}
	if(v_NumTextures > NORMAL)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D,m_CurrentNormalMap);
		int textureLocation = glGetUniformLocation(m_CurrentShader->getShaderLocation(),"normalMap");
		glUniform1i(textureLocation,NORMAL);
		//fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));
	}
	if(v_NumTextures > DEPTH)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D,m_CurrentDepthMap);
		int textureLocation = glGetUniformLocation(m_CurrentShader->getShaderLocation(),"depthMap");
		glUniform1i(textureLocation,DEPTH);
		//fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));
	}
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
				//fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));
				break;
			}
		case(DEBUG):
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_CULL_FACE);
				//fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));
				break;
			}
		case(FILLED):
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_CULL_FACE);
				//fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));
				break;
			}
		}
	}
		//draw mesh
	glBindVertexArray(m_CurrentMesh);	// Bind mesh VAO
	//fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));
	glDrawElements(GL_TRIANGLES, m_CurrentMeshVerts,  GL_UNSIGNED_INT, 0);	// draw VAO 
	//fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));
	glBindVertexArray(0);
	//disable shader and clear flags
	m_CurrentShader->disable();
	for (size_t i = 0;i<4;i++)
	{
		m_Textures[i] = false;
	}
	m_Matrices[MODEL] = false;
	m_Matrices[NORMALMATRIX] = false;
}
void GLTestRender::endRenderCycle(void)
{
	m_Matrices[VIEW] = false;
	m_Matrices[PROJECTION] = false;
	SDL_GL_SwapWindow(m_Window); // swap buffers
}
void GLTestRender::render(TextureNode* p_TextureNode)
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
void GLTestRender::render(TransformNode* p_Transform)
{
	m_CurrentModelMatrix = p_Transform->getWorldTransform();
	m_Matrices[MODEL] = true;
}
void GLTestRender::render(MeshNode* p_Mesh)
{
	m_CurrentMesh = p_Mesh->getMeshLocation();
	m_CurrentMeshVerts = p_Mesh->getNumVerts();
	m_MeshFlag = true;
}
void GLTestRender::render(MaterialNode* p_Material)
{
	m_CurrentMaterial = p_Material->getMaterialData();
}
void GLTestRender::render(CameraNode* p_CameraNode)
{
	p_CameraNode->getViewMatrix(m_CurrentViewMatrix,false);
	m_Matrices[VIEW] = true;
}
SDL_Window* GLTestRender::getWindow(void)
{
	return m_Window;
}
void GLTestRender::init(void)
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
   m_Window = SDL_CreateWindow("Tanked", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
	//enable standard opengl rendering features
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate
	fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));
	
}
void GLTestRender::shutDown(void)
{
    SDL_GL_DeleteContext(m_Context);
    SDL_DestroyWindow(m_Window);
	m_Shaders.clear();
	m_Lights.clear();
}
GLTestRender::~GLTestRender(void)
{
	m_Window = nullptr;
}
