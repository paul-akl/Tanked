#include "WeaponGauge.h"
#include "GeneralUtils.h"
#include "TextElement.h"
#include "GFXElement.h"

WeaponGauge::WeaponGauge(): m_CurrentValue(0), m_MaxValue(0), m_ScaleFactor(0), charging(false)
{
	
}


WeaponGauge::~WeaponGauge(void)
{
}

void WeaponGauge::init()
{
	//font = Utils::GeneralUtils::generateFont(font,"Fonts/ka1.ttf"); 
	TransformNode * Transform = new TransformNode();
	Transform->setName("weapon gauge transform");
	Transform->reset();	
	SceneNode::addTransform(Transform);

	MeshNode * Mesh = new MeshNode();
	Mesh->setName("uimesh");
	Mesh->loadModel("Models/plane2.obj");
	
	m_Gauge = new TextureNode();
	m_Gauge->setName("gauge");
	m_Gauge->setTextureType(DIFFUSE);
	m_Gauge->loadTexture("Images/chargegauge.png");

	m_GaugeBg = new TextureNode();
	m_GaugeBg->setName("gaugebg");
	m_GaugeBg->setTextureType(DIFFUSE);
	m_GaugeBg->loadTexture("Images/chargebar.png");

	m_ChargeBg = new GFXElement();
	m_ChargeBg->setPosition(glm::vec3(0.28,0.0,0.0));
	m_ChargeBg->setRenderRadius(glm::vec3(0.0,0.0,1.0));
	m_ChargeBg->setName("chargebg");
	m_ChargeBg->addMesh(Mesh);	
	m_ChargeBg->addTexture(m_GaugeBg);
	Transform = new TransformNode();
	Transform->setName("weapon image transform");
	Transform->reset();
	m_ChargeBg->addTransform(Transform);
	SceneNode::addNode(m_ChargeBg);

	m_ChargeFg = new GFXElement();
	m_ChargeFg->setPosition(glm::vec3(0.03f,0.0f,0.0f));
	m_ChargeFg->setName("chargefg");
	m_ChargeFg->addMesh(Mesh);	
	m_ChargeFg->addTexture(m_Gauge);
	Transform = new TransformNode();
	Transform->setName("weapon image transform");
	Transform->reset();
	m_ChargeFg->addTransform(Transform);
	SceneNode::addNode(m_ChargeFg);

	/*m_Ammo = new TextElement();
	m_Ammo->setName("shield value");
	m_Ammo->setRenderRadius(glm::vec3(0.2));
	m_Ammo->setFont(font);
	m_Ammo->setTextColour(glm::vec4(0,255,0,0));
	m_Ammo->addMesh(Mesh);
	TextureNode* shieldTexgreen = new TextureNode();
	shieldTexgreen->setName("valuetex");
	shieldTexgreen->setTextureType(DIFFUSE);	
	m_Ammo->addTexture(shieldTexgreen);
	Transform = new TransformNode();
	Transform->setName("shield value transform");
	Transform->reset();
	m_Ammo->addTransform(Transform);
	SceneNode::addNode(m_Ammo);*/
		
}

void WeaponGauge::update(float p_DeltaTimeS)// pass in a reference to the tank?
{
	m_ScaleFactor = (m_CurrentValue/m_MaxValue);
	
	float offset = 0.004f * m_ScaleFactor;
	
	if(m_CurrentValue == 0)
	{
		m_ChargeBg->setPosition(glm::vec3(0.06-m_ChargeFg->getRenderRadius().x,0.0,0.0));
		m_ChargeBg->setRenderRadius(glm::vec3(0.0,0.0,1.0));
	}
	
	else if(m_CurrentValue > 0 && m_CurrentValue < m_MaxValue)
	{
		m_ChargeBg->setPosition(glm::vec3( m_ChargeBg->getRenderRadius().x - m_ChargeBg->getLocation().x, m_ChargeBg->getLocation().y, m_ChargeBg->getLocation().z));
		m_ChargeBg->setRenderRadius(glm::vec3(m_CurrentValue-offset,1.0,1.0));
	}
	else if(m_CurrentValue >= m_MaxValue)
		m_ChargeBg->setPosition(glm::vec3(m_ChargeBg->getLocation()));

	UIElement::update(p_DeltaTimeS);	
	}

void WeaponGauge::render(Renderer * p_Renderer)
{
	m_ChargeBg->render(p_Renderer);
	m_ChargeFg->render(p_Renderer);
}

