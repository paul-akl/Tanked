#include "GBuffer.h"

GBuffer::GBuffer(int p_WindowWidth, int p_WindowHeight)
{
	m_status = 0;
	m_FboHandle = 0;
	m_BlurBuffer = 0;
	m_ColourHandle0 = 0;
	m_DepthBufferHandle = 0;

	m_WindowWidth	= p_WindowWidth;
	m_WindowHeight	= p_WindowHeight;
	m_finalPassBuffer = GBufferFinal;
	
	for(int i=0; i < GBufferNumTextures; i++)		// For each buffer
		m_TexBuffers[i] = GL_COLOR_ATTACHMENT0 + i;	// set up it's position in the shader

	m_EmissiveAndFinalBuffers[0] = GL_COLOR_ATTACHMENT0 + GBufferEmissive;
	m_EmissiveAndFinalBuffers[1] = GL_COLOR_ATTACHMENT0 + GBufferFinal;
}
GBuffer::~GBuffer(void)
{
	// Clean the existing m_buffers
    if(m_FboHandle != 0)
        glDeleteFramebuffers(1, &m_FboHandle);

    if(m_GBTextures[0] != 0)
        glDeleteTextures(sizeof(m_GBTextures) / sizeof(*m_GBTextures), m_GBTextures);

	if(m_DepthBufferHandle != 0)
		glDeleteTextures(1, &m_DepthBufferHandle);
	
	if(m_BlurBuffer != 0)
		glDeleteTextures(1, &m_BlurBuffer);
	
	if(m_FinalBuffer != 0)
		glDeleteTextures(1, &m_FinalBuffer);
}

void GBuffer::init()
{	
	// Create the FBO
	glGenFramebuffers(1, &m_FboHandle);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FboHandle);

	// Create geometry pass buffers
	glGenTextures(GBufferNumTextures, m_GBTextures);
	glGenTextures(1, &m_DepthBufferHandle);
	glGenTextures(1, &m_BlurBuffer);
	glGenTextures(1, &m_FinalBuffer);
	
	//GLint internalFormats[GBufferNumTextures];
	//GLenum textureFormats[GBufferNumTextures];
	//GLenum textureTypes[GBufferNumTextures];

	internalFormats[GBufferPosition] = GL_RGBA32F;
	textureFormats[GBufferPosition] = GL_RGBA;
	textureTypes[GBufferPosition] = GL_FLOAT;

	internalFormats[GBufferDiffuse] = GL_RGBA16F;
	textureFormats[GBufferDiffuse] = GL_RGBA;
	textureTypes[GBufferDiffuse] = GL_FLOAT;

	internalFormats[GBufferNormal] = GL_RGBA16F;
	textureFormats[GBufferNormal] = GL_RGBA;
	textureTypes[GBufferNormal] = GL_FLOAT;

	internalFormats[GBufferEmissive] = GL_RGBA16F;
	textureFormats[GBufferEmissive] = GL_RGBA;
	textureTypes[GBufferEmissive] = GL_FLOAT;

	// Create textures
	for(int i=0; i < GBufferNumTextures; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_GBTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormats[i], m_WindowWidth, m_WindowHeight, 0, textureFormats[i], textureTypes[i], NULL);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_GBTextures[i], 0);
	}
	
	// Create depth buffer
	glBindTexture(GL_TEXTURE_2D, m_DepthBufferHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_WindowWidth, m_WindowHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthBufferHandle, 0);

	// Create the blur buffer, that acts as an intermediate buffer between vertical and horizontal blur passes
	glBindTexture(GL_TEXTURE_2D, m_BlurBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_WindowWidth, m_WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + GBufferBlur, GL_TEXTURE_2D, m_BlurBuffer, 0);

	// Create the final buffer, that gets renderred to the screen
	glBindTexture(GL_TEXTURE_2D, m_FinalBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_WindowWidth, m_WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + GBufferFinal, GL_TEXTURE_2D, m_FinalBuffer, 0);

	// Check for errors and throw an exception in case of one
	m_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(m_status != GL_FRAMEBUFFER_COMPLETE)
		throw new std::exception("Geometry buffer has failed to load");

	// Restore the default FBO, so it doesn't get changed from the outside of the class
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
void GBuffer::reload(unsigned int p_windowWidth, unsigned int p_windowHeight)
{
	m_WindowWidth	= p_windowWidth;
	m_WindowHeight	= p_windowHeight;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FboHandle);

	// Resize geometry textures
	for(int i=0; i < GBufferNumTextures; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_GBTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormats[i], m_WindowWidth, m_WindowHeight, 0, textureFormats[i], textureTypes[i], NULL);
	}
	
	// Resize depth buffer
	glBindTexture(GL_TEXTURE_2D, m_DepthBufferHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_WindowWidth, p_windowHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);	// For AMD GPUs at labs

	// Resize the blur buffer
	glBindTexture(GL_TEXTURE_2D, m_BlurBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_WindowWidth, p_windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);

	// Resize the final buffer
	glBindTexture(GL_TEXTURE_2D, m_FinalBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_WindowWidth, p_windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
}

void GBuffer::initFrame()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FboHandle);	
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + GBufferBlur);	// Bind final buffer
    glClear(GL_COLOR_BUFFER_BIT);						// and clear it
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + GBufferFinal);	// Bind final buffer
    glClear(GL_COLOR_BUFFER_BIT);						// and clear it
}
void GBuffer::initGeometryPass()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FboHandle);
    glDrawBuffers(GBufferNumTextures, m_TexBuffers);		// Bind geometry pass buffers to write to
}
void GBuffer::initLightPass()
{	
	glActiveTexture(GL_TEXTURE0 + GBufferPosition);
	glBindTexture(GL_TEXTURE_2D, m_GBTextures[GBufferPosition]);
	
	glActiveTexture(GL_TEXTURE0 + GBufferDiffuse);
	glBindTexture(GL_TEXTURE_2D, m_GBTextures[GBufferDiffuse]);
	
	glActiveTexture(GL_TEXTURE0 + GBufferNormal);
	glBindTexture(GL_TEXTURE_2D, m_GBTextures[GBufferNormal]);

	glDrawBuffers(2, m_EmissiveAndFinalBuffers);
}
void GBuffer::initParticlePass()
{
	glDrawBuffers(2, m_EmissiveAndFinalBuffers);
}
void GBuffer::initFinalPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);				// Set default framebuffer to paste to
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FboHandle);	// Set GBuffer's framebuffer to copy from
    glReadBuffer(GL_COLOR_ATTACHMENT0 + m_finalPassBuffer);		// Bind final buffer, to copy from
	// ^
	// L <-- FOR DEBUG PURPOSES we are attaching a 1's buffer (diffuse buffer), instead of the 'GL_COLOR_ATTACHMENT0 + GBufferFinal' (final buffer)
}

void GBuffer::bindForReading(GBufferTextureType p_buffer, int p_activeTexture)
{
	glActiveTexture(GL_TEXTURE0 + p_activeTexture);
	switch (p_buffer)
	{
	case GBuffer::GBufferPosition:
		glBindTexture(GL_TEXTURE_2D, m_GBTextures[p_buffer]);
		break;
	case GBuffer::GBufferDiffuse:
		glBindTexture(GL_TEXTURE_2D, m_GBTextures[p_buffer]);
		break;
	case GBuffer::GBufferNormal:
		glBindTexture(GL_TEXTURE_2D, m_GBTextures[p_buffer]);
		break;
	case GBuffer::GBufferEmissive:
		glBindTexture(GL_TEXTURE_2D, m_GBTextures[p_buffer]);
		break;
	case GBuffer::GBufferFinal:
		glBindTexture(GL_TEXTURE_2D, m_FinalBuffer);
		break;
	case GBuffer::GBufferBlur:
		glBindTexture(GL_TEXTURE_2D, m_BlurBuffer);
		break;
	default:
		//glBindTexture(GL_TEXTURE_2D, 0);
		break;
	}
}
void GBuffer::bindForReading(GBufferTextureType p_buffer)
{
	glActiveTexture(GL_TEXTURE0);
	switch (p_buffer)
	{
	case GBuffer::GBufferPosition:
		glBindTexture(GL_TEXTURE_2D, m_GBTextures[p_buffer]);
		break;
	case GBuffer::GBufferDiffuse:
		glBindTexture(GL_TEXTURE_2D, m_GBTextures[p_buffer]);
		break;
	case GBuffer::GBufferNormal:
		glBindTexture(GL_TEXTURE_2D, m_GBTextures[p_buffer]);
		break;
	case GBuffer::GBufferEmissive:
		glBindTexture(GL_TEXTURE_2D, m_GBTextures[p_buffer]);
		break;
	case GBuffer::GBufferFinal:
		glBindTexture(GL_TEXTURE_2D, m_FinalBuffer);
		break;
	case GBuffer::GBufferBlur:
		glBindTexture(GL_TEXTURE_2D, m_BlurBuffer);
		break;
	default:
		//glBindTexture(GL_TEXTURE_2D, 0);
		break;
	}
}
void GBuffer::bindForWriting(GBufferTextureType p_buffer)
{
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + p_buffer);
}