#pragma once
#include "Notification.h"
#include<list>

class NotificationFactory
{
public:
	NotificationFactory(void);
	virtual ~NotificationFactory(void);
	void init();
	static Notification* createNotification(std::string p_Message);
	static Notification* createNotification(std::string p_Message, glm::vec3 p_Position);
	static Notification* createNotification(std::string p_Message, glm::vec3 p_Position, glm::vec3 p_Scale);
	static std::list<Notification*> m_NotificationList;
private:
	static Notification* m_Instance;
	static TextureNode* m_WindowBg;
	static MeshNode* m_WindowMesh;
	static TTF_Font* m_MessageFont;
	
};

