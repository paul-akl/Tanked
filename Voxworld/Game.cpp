#include "Game.h"
#include "IntroGameMode.h"
#include "MainGameMode.h"
#include "MenuMode.h"
#include "CreditsMode.h"
#include "GeneralUtils.h"
#include "DeferredRenderer.h"
#include <ctime>
#include <GL\GLU.h>
#include "Controller.h"

Game::Game(void)
{
}
void Game::init()
{
	m_Renderer = new DeferredRenderer(800,600);
	m_Renderer->init();
	m_Window = m_Renderer->getWindow();
	//initialise the stored pointers
	m_IntroMode = new IntroGameMode();
	m_MainMode = new MainGameMode();
	m_MenuMode = new MenuMode();
	m_CreditsMode = new CreditsMode();
	//m_ShopMode = new ShopMode);
	//and initialise our game Modes
	m_IntroMode->init(*this);
	m_MainMode->init(*this);
	m_MenuMode->init(*this);
	m_CreditsMode->init(*this);
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
	Controller* controller = new Controller();
	m_CurrentMode->setController(controller);
	controller->setWindow(m_Renderer->getWindow());
	//TO DO: write this method
	SDL_Event sdlEvent; // variable to detect SDL events

	bool running = true;
	float deltaTimeS = 0.0f;
	float timeMS = clock();
	float lastTimeMS = 0.0f;
	while (running)	
	{	// the event loop
		timeMS = clock();
		deltaTimeS = timeMS-lastTimeMS;
		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_QUIT)
				running = false;
			else
			{
				controller->update(sdlEvent);
			}
		}
		running = m_CurrentMode->handleEvent(*this);
		if(running)
		{
			//	then update the current Game Mode with elapsed time as parameter
			m_CurrentMode->update(deltaTimeS);
			//	and draw the resultant changes.
			m_CurrentMode->draw(m_Renderer);
			lastTimeMS = timeMS;
		}
	}
}
Game::~Game(void)
{
	//free the stored pointers
	delete m_IntroMode;
	delete m_MenuMode;
	delete m_MainMode;
	delete m_CreditsMode;
	//delete m_ShopMode;
	m_Window = nullptr;
	delete m_Renderer;
	printf("destroy game \n");
}
