#include "Game.h"
#include "IntroGameMode.h"
#include "MainGameMode.h"
#include "MenuMode.h"
#include "CreditsMode.h"
#include "GeneralUtils.h"
#include "DeferredRenderer.h"
#include "Renderer.h"
#include <ctime>
#include <GL\GLU.h>
#include "Controller.h"
#include "Locator.h"
Game::Game(void)
{
}
void Game::init()
{
	//create asset manager here
	
	Utils::GeneralUtils * util = new Utils::GeneralUtils();
	util->initialiseTTF();
	m_Renderer = new DeferredRenderer(1024, 768);
	m_Renderer->init();
	m_Window = m_Renderer->getWindow();
	//initialise asset system
	m_AssetManager = new AssetManager();
	Locator::provide(m_AssetManager->getAudioSystem());
	//initialise notifications
	m_NotificationSystem = new NotificationFactory();
	m_NotificationSystem->init();
	//initialise the stored pointers
	m_IntroMode = new IntroGameMode();
	m_MainMode = new MainGameMode();
	m_MenuMode = new MenuMode();
	m_CreditsMode = new CreditsMode();
	//m_ShopMode = new ShopMode);
	//and initialise our game Mode
	m_Controller = new Controller();
	m_MainMode->setController(m_Controller);
	//m_IntroMode->init(*this,m_Renderer);
	m_MainMode->init(*this,m_Renderer);
	//m_MenuMode->init(*this,m_Renderer);
	//m_CreditsMode->init(*this,m_Renderer);
	m_Controller->setWindow(m_Renderer->getWindow());
	//mShopMode->init(*this);
	//and set our default starting mode, which would normally be intro mode, but for now we will stick with main mode.
	m_CurrentMode = m_MainMode;

	
}
void Game::changeState(GameState state)
{
	//this part will be expanded as MenuMode and CreditsMode are created and implemented (and necessary)
	switch(state)
	{
	case INTRO:
		m_CurrentMode = m_IntroMode;
		break;
	case MAIN:
		m_CurrentMode = m_MainMode;
		break;
	case MENU:
		break;
	case CREDITS:
		break;
	};
}

void Game::Run()
{

	//TO DO: write this method
	SDL_Event sdlEvent; // variable to detect SDL events

	bool running = true;
	float deltaTimeS = 0.0f;
	float timeMS = (float)clock();
	float lastTimeMS = 0.0f;
	m_Renderer->beginRenderCycle(DEBUG);
	while (running)	
	{	// the event loop
		timeMS = (float)clock();
		deltaTimeS = timeMS-lastTimeMS;
		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_QUIT)
				running = false;
			else
			{
				m_Controller->update(sdlEvent);
			}
		}
		running = m_CurrentMode->handleEvent(*this);
		if(running)
		{
			int num = 0;
			//	then update the current Game Mode with elapsed time as parameter
			//DefferredRenderBlock block;
			//block.renderer = m_Renderer;
			//m_RenderThread= SDL_CreateThread(RenderScene,"render thread",&m_Renderer);
			m_CurrentMode->update(deltaTimeS);
			//update asset system (used for updating fmod)
			m_AssetManager->updateSubSystems();

			//SDL_WaitThread(m_RenderThread,&num);	
			//	and draw the resultant changes.

			m_CurrentMode->draw(m_Renderer);
			lastTimeMS = timeMS;
		}
	}
}
int Game::RenderScene(void* p_Renderer)
{
	DeferredRenderer* p_RendererObject = static_cast<DeferredRenderer*>(p_Renderer);

	p_RendererObject->endRenderCycle();
	return 0;
}
Game::~Game(void)
{
	//free the stored pointers
	delete m_IntroMode;
	delete m_MenuMode;
	delete m_MainMode;
	delete m_CreditsMode;
	//delete m_ShopMode;
	delete m_AssetManager;
	delete m_NotificationSystem;
	m_Window = nullptr;
	delete m_Renderer;
	printf("destroy game \n");
}
