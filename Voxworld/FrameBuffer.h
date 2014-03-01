#pragma once
#include <GL\glew.h>
#include <iostream>
class FrameBuffer
{
public:
	FrameBuffer(void);
	FrameBuffer(unsigned int p_windowWidth, unsigned int p_windowHeight);
	virtual ~FrameBuffer(void);

	virtual void Enable(){;}
	virtual void Disable(){;}
	virtual void init(){;}
	virtual void reload(unsigned int p_windowWidth, unsigned int p_windowHeight)=0;
	GLuint getTexture(GLenum* p_bufferId){;}
	void setRenderTexture(GLenum*  p_bufferId, GLuint textureHandle){;}
	bool checkStatus();
protected:
	GLenum m_status;
	GLuint m_FboHandle;
	GLuint m_ColourHandle0;
	GLuint m_DepthBufferHandle;
	unsigned int m_WindowWidth, m_WindowHeight;
};

