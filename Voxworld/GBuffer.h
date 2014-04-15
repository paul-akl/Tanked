//////////////////////////////////////////////////
//This inherits from FrameBuffer base class		//
//go ahead and change what you need. 			//	
//just keep in mind the functionality used in	//
//the DeferredRenderer class					//
//////////////////////////////////////////////////

#pragma once
#include "framebuffer.h"

class GBuffer : public FrameBuffer
{
public:

	enum GBufferTextureType
	{
		GBufferPosition,
		GBufferDiffuse,
		GBufferNormal,
		GBufferTexCoord,
		GBufferNumTextures
	};

	GBuffer(int p_WindowWidth, int p_WindowHeight);
	virtual ~GBuffer(void);
	virtual void enable(){glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FboHandle);}
	virtual void disable(){glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);}
	virtual void setReadBuffer(GBufferTextureType type){glReadBuffer(GL_COLOR_ATTACHMENT0 + type);}
	virtual void bindReadBuffer(){glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FboHandle);}

	virtual void init();				// Generates buffers, set's up FBO
	virtual void reload(unsigned int p_windowWidth, unsigned int p_windowHeight);

	virtual void initFrame();			// This should be called every frame, to clear the final buffer
	virtual void initGeometryPass();	// Bind geometry buffers for drawing
	virtual void initStencilPass();		// Prepare for light's stencil pass
	virtual void initLightPass();		// Bind buffers from geometry pass so they can be accessed when rendering lights
	virtual void initFinalPass();		// Bind the final buffer to 'read from' and the default screen buffer to 'write to'

	virtual GLuint getPositionBufferHandle(){		return GBufferPosition;	}
	virtual GLuint getDiffuseBufferHandle(){		return GBufferDiffuse;	}
	virtual GLuint getNormalBufferHandle(){			return GBufferNormal;		}
	virtual GLuint getTextureCoordBufferHandle(){	return GBufferTexCoord;	}

protected:

GLuint  m_GBTextures[GBufferNumTextures],	// Geometry pass textures
		m_FinalBuffer;						// Final buffer that gets copied to the screen
GLenum  m_TexBuffers[GBufferNumTextures];	// Handles for binding geometry buffers

};