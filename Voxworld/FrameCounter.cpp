#include "FrameCounter.h"
#include <iostream>
#include "GeneralUtils.h"
#include "TextElement.h"
#include <ctime>

FrameCounter::FrameCounter(): m_CurrentValue(0), currentTime(0), deltaTime(0), lastTime(0)
{
	
}


FrameCounter::~FrameCounter(void)
{
}

void FrameCounter::init()// add uielement factory
{	
	// request fonts from asset manager
	font = Utils::GeneralUtils::generateFont(font,"Fonts/ka1.ttf"); 
	TransformNode * Transform = new TransformNode();
	Transform->setName("FrameCounter transform");
	Transform->reset();	
	SceneNode::addTransform(Transform);

	MeshNode * Mesh = new MeshNode();
	Mesh->setName("uimesh");
	Mesh->loadModel("Models/plane2.obj");
	
	m_FrameCounter = new TextElement();
	m_FrameCounter->setName("FrameCounter value");
	m_FrameCounter->setRenderRadius(glm::vec3(1.0));
	m_FrameCounter->setFont(Utils::GeneralUtils::generateFont(m_FrameCounter->getFont(),"Fonts/Arkitech Medium.ttf"));
	m_FrameCounter->setTextColour(glm::vec4(255,0,0,0));
	m_FrameCounter->addMesh(Mesh);
	TextureNode* FrameCounterTexture = new TextureNode();
	FrameCounterTexture->setName("FrameCountertex");
	FrameCounterTexture->setTextureType(DIFFUSE);	
	m_FrameCounter->addTexture(FrameCounterTexture);
	Transform = new TransformNode();
	Transform->setName("FrameCounter transform");
	Transform->reset();
	m_FrameCounter->addTransform(Transform);
	SceneNode::addNode(m_FrameCounter);
			
}

void FrameCounter::update(float p_DeltaTimeS)
{
		
	std::stringstream ss;
	ss <<"ms/f : "<< m_CurrentValue;
	m_FrameCounter->setText(ss.str().c_str());
	ss.clear();

	UIElement::update(p_DeltaTimeS);	
	SceneNode::update(p_DeltaTimeS);
	
}

void FrameCounter::render(Renderer * p_Renderer)
{	
	m_FrameCounter->render(p_Renderer);
	
}