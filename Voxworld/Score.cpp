#include "Score.h"
#include <iostream>
#include "GeneralUtils.h"
#include "TextElement.h"
#include "GFXElement.h"

Score::Score(): m_CurrentValue(0)
{
	
}


Score::~Score(void)
{
}

void Score::init()// add uielement factory
{	
	// request fonts from asset manager
	font = Utils::GeneralUtils::generateFont(font,"Fonts/ka1.ttf"); 
	TransformNode * Transform = new TransformNode();
	Transform->setName("score transform");
	Transform->reset();	
	SceneNode::addTransform(Transform);

	MeshNode * Mesh = new MeshNode();
	Mesh->setName("uimesh");
	Mesh->loadModel("Models/plane2.obj");
	
	m_Score = new TextElement();
	m_Score->setName("score value");
	m_Score->setRenderRadius(glm::vec3(1.0));
	m_Score->setFont(font);
	m_Score->setTextColour(glm::vec4(255,255,255,0));
	m_Score->addMesh(Mesh);
	TextureNode* scoreTexture = new TextureNode();
	scoreTexture->setName("scoretex");
	scoreTexture->setTextureType(DIFFUSE);	
	m_Score->addTexture(scoreTexture);
	Transform = new TransformNode();
	Transform->setName("shield value transform");
	Transform->reset();
	m_Score->addTransform(Transform);
	SceneNode::addNode(m_Score);
			
}

void Score::update(float p_DeltaTimeS)// pass in a reference to the tank?
{
	
	std::stringstream ss;
	ss << "Score: " << (int)m_CurrentValue;	
	m_Score->setText(ss.str().c_str());
	ss.clear();

	UIElement::update(p_DeltaTimeS);	
	SceneNode::update(p_DeltaTimeS);
}

void Score::render(Renderer * p_Renderer)
{	
	m_Score->render(p_Renderer);
	
}