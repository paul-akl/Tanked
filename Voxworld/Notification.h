#pragma once
#include "UIWindow.h"
#include "GFXElement.h"
#include "TextElement.h"
class Notification :
	public UIWindow
{
public:
	Notification(void);
	virtual ~Notification(void);
	virtual void init();
	virtual void update(float p_DeltaTimeS);
	virtual void render(Renderer * p_Renderer);
	bool timeOut();
private:
	GFXElement* m_WindowImage;
	TextElement* m_Text;
	friend class NotificationFactory;
	int m_LifeTime;
};

