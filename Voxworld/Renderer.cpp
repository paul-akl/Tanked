#include "Renderer.h"


Renderer::Renderer(int p_ScreenWidth, int p_ScreenHeight)
{
	m_FOV = 60.0f;
	m_zFar = 400.0f;
	m_zNear = 1.0;
	m_CurrentScreenWidth = p_ScreenWidth;
	m_CurrentScreenHeight = p_ScreenHeight;
	m_WindowedScrenWidth = p_ScreenWidth;
	m_WindowedScreenHeight = p_ScreenHeight;
	m_FullscreenWidth = p_ScreenWidth;
	m_FullscreenHeight = p_ScreenHeight;
	m_Fullscreen = false;
}


Renderer::~Renderer(void)
{
}
