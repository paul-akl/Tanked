#include "NotificationFactory.h"
#include "TextureNode.h"
#include "MeshNode.h"
#include "TransformNode.h"
#include "GeneralUtils.h"

Notification* NotificationFactory::m_Instance= nullptr;
TextureNode* NotificationFactory::m_WindowBg = nullptr;
MeshNode* NotificationFactory::m_WindowMesh = nullptr;
TTF_Font* NotificationFactory::m_MessageFont = nullptr;

std::list<Notification*> NotificationFactory::m_NotificationList;

NotificationFactory::NotificationFactory(void)
{
	
}

void NotificationFactory::init()
{
	m_MessageFont = Utils::GeneralUtils::generateFont(m_MessageFont,"Fonts/Arkitech Medium.ttf"); 
	m_WindowBg = new TextureNode();
	m_WindowBg->setTextureType(DIFFUSE);
	m_WindowBg->setName("windowbg");
	std::string v_FileName = "Images/futureuibox.png";// window image
	m_WindowBg->loadTexture(v_FileName);

	m_WindowMesh = new MeshNode();
	m_WindowMesh->setName("uimesh");
	m_WindowMesh->loadModel("Models/plane2.obj");

	
}
Notification* NotificationFactory::createNotification(std::string p_Message)
{
	//once a notification is created it will start a timer, once the is finished it will destroy itself
		
		TransformNode* Transform = new TransformNode();
		//set it's name
		Transform->setName("notificationtransform");
		//and set it to identity matrix
		Transform->reset();
		//instantiate a notification
		m_Instance = new Notification();
		m_Instance->init();
		//set name
		m_Instance->setName("notification");
		m_Instance->setPosition(glm::vec3(0.0f,0.5f,0.0f));
		m_Instance->setScale(glm::vec3(0.3f,0.1f,0.3f));
		m_Instance->setOrientation(0.0f);
		m_Instance->addTransform(Transform);
		TextureNode* Texture = new TextureNode();
		Texture->setName("notificationtex");
		Texture->setTextureType(DIFFUSE);	
		
		
		//graphics component
		m_Instance->m_WindowImage = new GFXElement();
		m_Instance->m_WindowImage->setScale(glm::vec3(1.0f));
		m_Instance->m_WindowImage->setName("bgimage");
		m_Instance->m_WindowImage->addMesh(m_WindowMesh);
		Transform = new TransformNode();
		Transform->reset();
		m_Instance->m_WindowImage->addTransform(Transform);
		m_Instance->m_WindowImage->addTexture(m_WindowBg);
		m_Instance->addNode(m_Instance->m_WindowImage);
		
		//text component
		m_Instance->m_Text = new TextElement();		
		m_Instance->m_Text->setScale(glm::vec3(0.4f,0.2f,1.0f));
		m_Instance->m_Text->addTexture(Texture);
		m_Instance->m_Text->setText(p_Message);
		m_Instance->m_Text->setFont(m_MessageFont);
		m_Instance->m_Text->setTextColour(glm::vec4(0,255,0,0));
		m_Instance->m_Text->createStaticText();		
		m_Instance->m_Text->addMesh(m_WindowMesh);
		Transform = new TransformNode();
		Transform->reset();
		m_Instance->m_Text->addTransform(Transform);
		m_Instance->addNode(m_Instance->m_Text);
	
		return m_Instance;
		
}

//create notification with user defined position
Notification* NotificationFactory::createNotification(std::string p_Message, glm::vec3 p_Position)
{
	//once a notification is created it will start a timer, once the is finished it will destroy itself
		
		TransformNode* Transform = new TransformNode();
		//set it's name
		Transform->setName("notificationtransform");
		//and set it to identity matrix
		Transform->reset();
		//instantiate a notification
		m_Instance = new Notification();
		m_Instance->init();
		//set name
		m_Instance->setName("notification");
		m_Instance->setPosition(p_Position);
		m_Instance->setScale(glm::vec3(0.3f,0.1f,0.3f));
		m_Instance->setOrientation(0.0f);
		m_Instance->addTransform(Transform);
		TextureNode* Texture = new TextureNode();
		Texture->setName("notificationtex");
		Texture->setTextureType(DIFFUSE);	
		
		
		//graphics component
		m_Instance->m_WindowImage = new GFXElement();
		m_Instance->m_WindowImage->setScale(glm::vec3(1.0f));
		m_Instance->m_WindowImage->setName("bgimage");
		m_Instance->m_WindowImage->addMesh(m_WindowMesh);
		Transform = new TransformNode();
		Transform->reset();
		m_Instance->m_WindowImage->addTransform(Transform);
		m_Instance->m_WindowImage->addTexture(m_WindowBg);
		m_Instance->addNode(m_Instance->m_WindowImage);
		
		//text component
		m_Instance->m_Text = new TextElement();		
		m_Instance->m_Text->setScale(glm::vec3(0.6f,0.3f,1.0f));
		m_Instance->m_Text->addTexture(Texture);
		m_Instance->m_Text->setText(p_Message);
		m_Instance->m_Text->setFont(m_MessageFont);
		m_Instance->m_Text->setTextColour(glm::vec4(0,255,0,0));
		m_Instance->m_Text->createStaticText();		
		m_Instance->m_Text->addMesh(m_WindowMesh);
		Transform = new TransformNode();
		Transform->reset();
		m_Instance->m_Text->addTransform(Transform);
		m_Instance->addNode(m_Instance->m_Text);
	
		return m_Instance;
		
}

//create notification with user defined position and scale
Notification* NotificationFactory::createNotification(std::string p_Message, glm::vec3 p_Position, glm::vec3 p_Scale)
{
	//once a notification is created it will start a timer, once the is finished it will destroy itself
		
		TransformNode* Transform = new TransformNode();
		//set it's name
		Transform->setName("notificationtransform");
		//and set it to identity matrix
		Transform->reset();
		//instantiate a notification
		m_Instance = new Notification();
		m_Instance->init();
		//set name
		m_Instance->setName("notification");
		m_Instance->setPosition(p_Position);
		m_Instance->setScale(p_Scale);
		m_Instance->setOrientation(0.0f);
		m_Instance->addTransform(Transform);
		TextureNode* Texture = new TextureNode();
		Texture->setName("notificationtex");
		Texture->setTextureType(DIFFUSE);	
		
		
		//graphics component
		m_Instance->m_WindowImage = new GFXElement();
		m_Instance->m_WindowImage->setScale(glm::vec3(1.0f));
		m_Instance->m_WindowImage->setName("bgimage");
		m_Instance->m_WindowImage->addMesh(m_WindowMesh);
		Transform = new TransformNode();
		Transform->reset();
		m_Instance->m_WindowImage->addTransform(Transform);
		m_Instance->m_WindowImage->addTexture(m_WindowBg);
		m_Instance->addNode(m_Instance->m_WindowImage);
		
		//text component
		m_Instance->m_Text = new TextElement();		
		m_Instance->m_Text->setScale(glm::vec3(0.4f,0.2f,1.0f));
		m_Instance->m_Text->addTexture(Texture);
		m_Instance->m_Text->setText(p_Message);
		m_Instance->m_Text->setFont(m_MessageFont);
		m_Instance->m_Text->setTextColour(glm::vec4(0,255,0,0));
		m_Instance->m_Text->createStaticText();		
		m_Instance->m_Text->addMesh(m_WindowMesh);
		Transform = new TransformNode();
		Transform->reset();
		m_Instance->m_Text->addTransform(Transform);
		m_Instance->addNode(m_Instance->m_Text);
	
		return m_Instance;
		
}

NotificationFactory::~NotificationFactory(void)
{
	
	m_MessageFont = nullptr;
	delete m_WindowBg;
	delete m_WindowMesh;
}
