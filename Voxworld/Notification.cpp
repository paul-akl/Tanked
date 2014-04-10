#include "Notification.h"
#include "GeneralUtils.h"


Notification::Notification(void): m_LifeTime(0), m_WindowImage(nullptr), m_Text(nullptr)
{
}


Notification::~Notification(void)
{
	delete m_WindowImage;
	delete m_Text;
}

void Notification::init()
{
	m_WindowImage = new GFXElement();
	m_Text = new TextElement();
}

void Notification::update(float p_DeltaTimeS)
{
	m_LifeTime ++;
	UIWindow::update(p_DeltaTimeS);
	
}

void Notification::render(Renderer * p_Renderer)
{
	m_WindowImage->render(p_Renderer);
	m_Text->render(p_Renderer);
}

bool Notification::timeOut()
{
	if( m_LifeTime > 30)
		return true;

	return false;
}