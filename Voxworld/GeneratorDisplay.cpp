#include "GeneratorDisplay.h"
#include <iostream>
#include "GeneralUtils.h"
#include "TextElement.h"
#include "GFXElement.h"

GeneratorDisplay::GeneratorDisplay(): m_CurrentValue(0)
{
	
}


GeneratorDisplay::~GeneratorDisplay(void)
{
}

void GeneratorDisplay::init()// add uielement factory
{	
	// request fonts from asset manager
	//font = Utils::GeneralUtils::generateFont(font,"Fonts/ka1.ttf"); 
	TransformNode * Transform = new TransformNode();
	Transform->setName("GeneratorDisplay transform");
	Transform->reset();	
	SceneNode::addTransform(Transform);

	MeshNode * Mesh = new MeshNode();
	Mesh->setName("uimesh");
	Mesh->loadModel("Models/plane2.obj");
	
	m_GeneratorDisplay = new TextElement();
	m_GeneratorDisplay->setName("GeneratorDisplay value");
	m_GeneratorDisplay->setScale(glm::vec3(1.0));
	m_GeneratorDisplay->setFont(Utils::GeneralUtils::generateFont(m_GeneratorDisplay->getFont(),"Fonts/Arkitech Medium.ttf"));
	m_GeneratorDisplay->setTextColour(glm::vec4(255,0,0,0));
	m_GeneratorDisplay->addMesh(Mesh);
	TextureNode* GeneratorDisplayTexture = new TextureNode();
	GeneratorDisplayTexture->setName("GeneratorDisplaytex");
	GeneratorDisplayTexture->setTextureType(DIFFUSE);	
	m_GeneratorDisplay->addTexture(GeneratorDisplayTexture);
	Transform = new TransformNode();
	Transform->setName("GeneratorDisplay transform");
	Transform->reset();
	m_GeneratorDisplay->addTransform(Transform);
	SceneNode::addNode(m_GeneratorDisplay);
			
}

void GeneratorDisplay::update(float p_DeltaTimeS)// pass in a reference to the tank?
{
	
	std::stringstream ss;
	ss << "Enemy Generators Remaining: " << (int)m_CurrentValue;	
	m_GeneratorDisplay->setText(ss.str().c_str());
	ss.clear();

	UIElement::update(p_DeltaTimeS);	
	SceneNode::update(p_DeltaTimeS);
}

void GeneratorDisplay::render(Renderer * p_Renderer)
{	
	m_GeneratorDisplay->render(p_Renderer);
	
}