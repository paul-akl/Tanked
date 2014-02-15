#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(void)
{
}

FrameBuffer::FrameBuffer(unsigned int WindowWidth, unsigned int WindowHeight)
{
	m_WindowWidth = WindowWidth;
	m_WindowHeight = WindowHeight;
}

FrameBuffer::~FrameBuffer(void)
{
}

bool FrameBuffer::checkStatus()
{
	m_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(m_status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error, status: 0x%x\n", m_status);
		return false;
	}
	return true;
}