#pragma once
#include <GL\glew.h>
#include "GameMode.h"

struct DefferredRenderBlock
{
	Renderer* renderer;
};
class Game
{
public:
	Game(void);
	void init();
	void Run();
	~Game(void);
private:
	friend class GameMode;
	void changeState(GameState state);
	GameMode* m_IntroMode;
	GameMode* m_MenuMode;
	GameMode* m_MainMode;
	GameMode* m_CreditsMode;
	//GameMode* m_ShopMode;
	GameMode* m_CurrentMode;
	Renderer* m_Renderer;
	SDL_Thread* m_RenderThread;
	SDL_Window* m_Window;
	Controller* m_Controller;
	static int RenderScene(void* p_Renderer);
	//Controller* m_Controller

};

