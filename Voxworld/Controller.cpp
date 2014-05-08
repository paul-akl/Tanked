#include "Controller.h"
#include <SDL.h>

Controller::Controller(void)
{
	for (int i = 0; i < 256; i++)
	{
		keyBuffer[i]= false;
	}
	mouseX = 0;
	mouseY = 0;

}
void Controller::setGameMode(bool p_Switch)
{
	SDL_SetRelativeMouseMode((SDL_bool)p_Switch);
	m_GameMode = p_Switch;
	mouseX = 0.0f;
	mouseY = 0.0;
	keyBuffer[MOUSERIGHT] = false;
	keyBuffer[MOUSELEFT] = false;
}	
float Controller::getMousePositionX()
{
	SDL_GetMouseState(&mouseX,&mouseY);
	return mouseX;
}
float Controller::getMousePositionY()
{
	SDL_GetMouseState(&mouseX,&mouseY);
	return mouseY;
}
bool Controller::update(SDL_Event& p_Event)
{
	//mouseX = 0.0f;
	//mouseY = 0.0f;
	//This needs updated. Is incomplete
	if(p_Event.type == SDL_KEYDOWN)
	{
		switch(p_Event.key.keysym.scancode)
		{
			case SDL_SCANCODE_ESCAPE:
			{
				keyBuffer[EXIT] = true;
			}
			break;
			case SDL_SCANCODE_P:
			{
				keyBuffer[PAUSE] = true;
			}
			break;
			case SDL_SCANCODE_Q:
			{
				keyBuffer[TURN_L] = true;
			}
			break;
			case SDL_SCANCODE_W:
			{
				keyBuffer[FORWARD] = true;
			}
			break;
			case SDL_SCANCODE_S:
			{
				keyBuffer[BACKPEDAL] = true;
			}
			break;
			case SDL_SCANCODE_A:
			{
				keyBuffer[STRAFE_L] = true;
			}
			break;
			case SDL_SCANCODE_D:
			{
				keyBuffer[STRAFE_R] = true;
			}
			break;
			case SDL_SCANCODE_E:
			{
				keyBuffer[TURN_R] = true;
			}
			break;	
			case SDL_SCANCODE_2:
			{
				keyBuffer[RENDERMODE_DEBUG] = true;
			}
			break;
			case SDL_SCANCODE_1:
			{
				keyBuffer[RENDERMODE_DEFAULT] = true;
			}
			break;
			case SDL_SCANCODE_RETURN:
			{
				keyBuffer[FULLSCREEN_KEY_A] = true;
			}
			break;
			case SDL_SCANCODE_LALT:
			{
				keyBuffer[FULLSCREEN_KEY_B] = true;
			}
			break;
			case SDL_SCANCODE_RALT:
			{
				keyBuffer[FULLSCREEN_KEY_B] = true;
			}
			break;
		}
	}
	if(p_Event.type == SDL_KEYUP)
	{
		switch(p_Event.key.keysym.scancode)
		{
			case SDL_SCANCODE_ESCAPE:
			{
				keyBuffer[EXIT] = false;
			}
			break;
			case SDL_SCANCODE_P:
			{
				keyBuffer[PAUSE] = false;
			}
			break;
			case SDL_SCANCODE_Q:
			{
				keyBuffer[TURN_L] = false;
			}
			break;
			case SDL_SCANCODE_E:
			{
				keyBuffer[TURN_R] = false;
			}
			break;
			case SDL_SCANCODE_2:
			{
				keyBuffer[RENDERMODE_DEBUG] = false;
			}
			break;
			case SDL_SCANCODE_1:
			{
				keyBuffer[RENDERMODE_DEFAULT] = false;
			}
			break;
			case SDL_SCANCODE_W:
			{
				keyBuffer[FORWARD] = false;
			}
			break;
			case SDL_SCANCODE_S:
			{
				keyBuffer[BACKPEDAL] = false;
			}
			break;
			case SDL_SCANCODE_A:
			{
				keyBuffer[STRAFE_L] = false;
			}
			break;
			case SDL_SCANCODE_D:
			{
				keyBuffer[STRAFE_R] = false;
			}
			break;
			case SDL_SCANCODE_RETURN:
			{
				keyBuffer[FULLSCREEN_KEY_A] = false;
			}
			break;
			case SDL_SCANCODE_LALT:
			{
				keyBuffer[FULLSCREEN_KEY_B] = false;
			}
			break;
			case SDL_SCANCODE_RALT:
			{
				keyBuffer[FULLSCREEN_KEY_B] = false;
			}
			break;
		}
	}
	if(p_Event.type == SDL_MOUSEWHEEL)
	{
		mouseWheelturn = (float)p_Event.wheel.y;
	}
	if(p_Event.type == SDL_MOUSEBUTTONDOWN)
	{
		if(p_Event.button.button == SDL_BUTTON_LEFT)
		{
			if(m_GameMode)
				keyBuffer[PRIMARY_ATTACK] = true;
			else
				keyBuffer[LMB] = true;

		}
		if(p_Event.button.button == SDL_BUTTON_RIGHT)
		{
			if(m_GameMode)
				keyBuffer[SECONDARY_ATTACK] = true;
			else
				keyBuffer[RMB] = true;
		}
	}
	if(p_Event.type == SDL_MOUSEBUTTONUP)
	{
		if(p_Event.button.button == SDL_BUTTON_LEFT)
		{
			keyBuffer[PRIMARY_ATTACK] = false;
		}
		if(p_Event.button.button == SDL_BUTTON_RIGHT)
		{
			keyBuffer[SECONDARY_ATTACK] = false;
		}
	}
	if(p_Event.type == SDL_MOUSEMOTION)
	{

		mouseX = p_Event.motion.xrel;
		if(mouseX<0)
		{
			keyBuffer[MOUSERIGHT] = false;
			keyBuffer[MOUSELEFT] = true;
		}
		else if(mouseX>0)
		{
			keyBuffer[MOUSERIGHT] = true;
			keyBuffer[MOUSELEFT] = false;
		}
		else
		{
			keyBuffer[MOUSERIGHT] = false;
			keyBuffer[MOUSELEFT] = false;
		}
	}
	else
	{
		mouseX = 0.0f;
		keyBuffer[MOUSERIGHT] = false;
		keyBuffer[MOUSELEFT] = false;
	}
	
	return true;
}
int Controller::getMouseMovementX()
{
		return mouseX;
}
int Controller::getMouseMovementY()
{
		return mouseY;
}
void Controller::resetMouse()
{
		keyBuffer[MOUSERIGHT] = false;
		mouseX = 0.0f;
		keyBuffer[MOUSELEFT] = false;
}
Controller::~Controller(void)
{
}