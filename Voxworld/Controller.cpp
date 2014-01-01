#include "Controller.h"
#include <SDL.h>

Controller::Controller(void)
{
	for (int i = 0; i < 256; i++)
	{
		keyBuffer[i]= false;
	}
	mouseX = 0.0f;
	mouseY = 0.0f;
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
		}
	}
	if(p_Event.type == SDL_MOUSEWHEEL)
	{
		mouseWheelturn = p_Event.wheel.y;
	}
	if(p_Event.type == SDL_MOUSEBUTTONDOWN)
	{
		if(p_Event.button.button == SDL_BUTTON_LEFT)
		{
			keyBuffer[PRIMARY_ATTACK] = true;
		}
		if(p_Event.button.button == SDL_BUTTON_RIGHT)
		{
			keyBuffer[SECONDARY_ATTACK] = true;
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

	
	return true;
}
int Controller::getMouseMovementX()
{
		lastMouseX = mouseX;
		lastMouseY = mouseY;
		SDL_GetMouseState(&mouseX,&mouseY);
		if(mouseX-lastMouseX > 0)
		{
			keyBuffer[MOUSELEFT] = false;
			keyBuffer[MOUSERIGHT] = true;
		}
		else if(mouseX-lastMouseX < 0)
		{
			keyBuffer[MOUSELEFT] = true;
			keyBuffer[MOUSERIGHT] = false;
		}
		else
		{
			keyBuffer[MOUSELEFT] = false;
			keyBuffer[MOUSERIGHT] = false;
		}
		return mouseX-lastMouseX;

}
int Controller::getMouseMovementY()
{
		lastMouseX = mouseX;
		lastMouseY = mouseY;
		SDL_GetMouseState(&mouseX,&mouseY);
		return mouseY-lastMouseY;
		SDL_WarpMouseInWindow(m_Window,400,300);

}
Controller::~Controller(void)
{
}