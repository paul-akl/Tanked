#include "GBuffer.h"

GBuffer::GBuffer(int p_WindowWidth, int p_WindowHeight)
{
	m_status = 0;
	m_FboHandle = 0;
	m_ColourHandle0 = 0;
	m_DepthBufferHandle = 0;

	m_WindowWidth	= p_WindowWidth;
	m_WindowHeight	= p_WindowHeight;
	
	for(int i=0; i < GBufferNumTextures; i++)		// For each buffer
		m_TexBuffers[i] = GL_COLOR_ATTACHMENT0 + i;	// set up it's position in the shader
}
GBuffer::~GBuffer(void)
{
}

void GBuffer::init()
{	
	// Create the FBO
	glGenFramebuffers(1, &m_FboHandle);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FboHandle);

	// Create geometry pass buffers
	glGenTextures(GBufferNumTextures, m_GBTextures);
	glGenTextures(1, &m_DepthBufferHandle);
	glGenTextures(1, &m_FinalBuffer);

	// Create textures
	for(int i=0; i < GBufferNumTextures; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_GBTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_WindowWidth, m_WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);

		// Using nearest filtering instead of linear, to avoid any distortions
		// and unnecessary interpolation (since the sampling is 1 to 1)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_GBTextures[i], 0);
	}
	
	// Create depth buffer
	glBindTexture(GL_TEXTURE_2D, m_DepthBufferHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_WindowWidth, m_WindowHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthBufferHandle, 0);

	// Create the final buffer, that gets renderred to the screen
	glBindTexture(GL_TEXTURE_2D, m_FinalBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_WindowWidth, m_WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_FinalBuffer, 0);

	// Check for errors and throw an exception in case of one
	m_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(m_status != GL_FRAMEBUFFER_COMPLETE)
		throw new std::exception("Geometry buffer has failed to load");

	// Restore the default FBO, so it doesn't get changed from the outside of the class
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
void GBuffer::reload(unsigned int p_windowWidth, unsigned int p_windowHeight)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FboHandle);

	// Resize geometry textures
	for(int i=0; i < GBufferNumTextures; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_GBTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, p_windowWidth, p_windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	}
	
	// Resize depth buffer
	glBindTexture(GL_TEXTURE_2D, m_DepthBufferHandle);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, p_windowWidth, p_windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, p_windowWidth, p_windowHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);	// For AMD GPUs at labs

	// Resize the final buffer
	glBindTexture(GL_TEXTURE_2D, m_FinalBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_windowWidth, p_windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
}

void GBuffer::initFrame()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FboHandle);	
	glDrawBuffer(GL_COLOR_ATTACHMENT4);	// Bind final buffer
    glClear(GL_COLOR_BUFFER_BIT);		// and clear it
}
void GBuffer::initGeometryPass()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FboHandle);
    glDrawBuffers(GBufferNumTextures, m_TexBuffers);		// Bind geometry pass buffers to write to
}
void GBuffer::initStencilPass()
{
	// Disable the draw buffers, since stencil pass uses the depth and stencil test only, no actual rendering
	glDrawBuffer(GL_NONE);
}
void GBuffer::initLightPass()
{
	glDrawBuffer(GL_COLOR_ATTACHMENT4);			// Bind final buffer to draw to
	
	for(int i=0; i < GBufferNumTextures; i++)							 // For each geometry buffer
	{
		glActiveTexture(GL_TEXTURE0 + i);								 // Make it active
		glBindTexture(GL_TEXTURE_2D, m_GBTextures[GBufferPosition + i]); // and bind it so it can be accessed in the shaders
	}
}
void GBuffer::initFinalPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);				// Set default framebuffer to paste to
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FboHandle);	// Set GBuffer's framebuffer to copy from
    glReadBuffer(GL_COLOR_ATTACHMENT0 + GBufferNumTextures);						// Bind final buffer, to copy from
	// ^
	// L <-- FOR DEBUG PURPOSES we are attaching a 1's buffer (diffuse buffer), instead of the 'GL_COLOR_ATTACHMENT4' (final buffer)
}