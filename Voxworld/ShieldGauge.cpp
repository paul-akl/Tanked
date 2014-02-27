#include "ShieldGauge.h"
#include <iostream>
#include "GeneralUtils.h"
#include "TextElement.h"
#include "GFXElement.h"

ShieldGauge::ShieldGauge(): m_CurrentValue(0), m_MaxValue(100), m_ScaleFactor(0)
{
	
}


ShieldGauge::~ShieldGauge(void)
{
}

void ShieldGauge::init()// add uielement factory
{	
	// request fonts from asset manager
	//font = Utils::GeneralUtils::generateFont(font,"Fonts/ka1.ttf"); 
	TransformNode * Transform = new TransformNode();
	Transform->setName("shield gauge transform");
	Transform->reset();	
	SceneNode::addTransform(Transform);

	MeshNode * Mesh = new MeshNode();
	Mesh->setName("uimesh");
	Mesh->loadModel("Models/plane2.obj");
	
	m_ShieldFull = new TextureNode();
	m_ShieldFull->setName("shieldtexgreen");
	m_ShieldFull->setTextureType(DIFFUSE);
	m_ShieldFull->loadTexture("Images/shieldfull.png");

	m_ShieldDamaged = new TextureNode();
	m_ShieldDamaged->setName("shieldtexgreen");
	m_ShieldDamaged->setTextureType(DIFFUSE);
	m_ShieldDamaged->loadTexture("Images/shield.png");

	m_ShieldImage = new GFXElement();
	m_ShieldImage->setName("shield image");
	m_ShieldImage->addMesh(Mesh);	
	m_ShieldImage->addTexture(m_ShieldFull);//get rid of these
	m_ShieldImage->addTexture(m_ShieldDamaged);
	Transform = new TransformNode();
	Transform->setName("shield image transform");
	Transform->reset();
	m_ShieldImage->addTransform(Transform);
	SceneNode::addNode(m_ShieldImage);

	m_TankImage = new GFXElement();
	m_TankImage->setName("tank image");
	m_TankImage->setRenderRadius(glm::vec3(0.5,0.7,0.5));
	m_TankImage->addMesh(Mesh);
	TextureNode* shieldTexgreen = new TextureNode();
	shieldTexgreen->setName("tanktex");
	shieldTexgreen->setTextureType(DIFFUSE);
	shieldTexgreen->loadTexture("Images/tankbg1.png");
	m_TankImage->addTexture(shieldTexgreen);
	Transform = new TransformNode();
	Transform->setName("tank image transform");
	Transform->reset();
	m_TankImage->addTransform(Transform);
	SceneNode::addNode(m_TankImage);

	m_ShieldValue = new TextElement();
	m_ShieldValue->setName("shield value");
	m_ShieldValue->setRenderRadius(glm::vec3(0.2));
	//Utils::GeneralUtils::generateFont(m_ShieldValue->getFont(),"Fonts/ka1.ttf"); 
	m_ShieldValue->setFont(Utils::GeneralUtils::generateFont(m_ShieldValue->getFont(),"Fonts/ka1.ttf"));
	m_ShieldValue->setTextColour(glm::vec4(0,255,0,0));
	m_ShieldValue->addMesh(Mesh);
	shieldTexgreen = new TextureNode();
	shieldTexgreen->setName("valuetex");
	shieldTexgreen->setTextureType(DIFFUSE);	
	m_ShieldValue->addTexture(shieldTexgreen);
	Transform = new TransformNode();
	Transform->setName("shield value transform");
	Transform->reset();
	m_ShieldValue->addTransform(Transform);
	SceneNode::addNode(m_ShieldValue);
			
}

void ShieldGauge::update(float p_DeltaTimeS)// pass in a reference to the tank?
{
	m_ScaleFactor = (m_CurrentValue/m_MaxValue);
	//m_ShieldValue->needsUpdate = true;
	std::stringstream ss;
	ss << (int)m_CurrentValue;	
	m_ShieldValue->setText(ss.str().c_str());
	ss.clear();

	if(m_CurrentValue == m_MaxValue)
	{
		m_ShieldImage->setTexture(m_ShieldFull);
	}
	else
	{
		m_ShieldImage->setTexture(m_ShieldDamaged);
	}

	m_ShieldImage->setRenderRadius(glm::vec3(m_ScaleFactor));
	m_ShieldValue->update(p_DeltaTimeS);
	UIElement::update(p_DeltaTimeS);	
	SceneNode::update(p_DeltaTimeS);
}

void ShieldGauge::render(Renderer * p_Renderer)
{	
	m_ShieldImage->render(p_Renderer);
	m_TankImage->render(p_Renderer);
	m_ShieldValue->render(p_Renderer);
}