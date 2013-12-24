#pragma once
#include <SDL.h>
enum ButtonType
{
	FORWARD,
	BACKPEDAL,
	STRAFE_L,
	STRAFE_R,
	TURN_L,
	TURN_R,
	JUMP,
	CROUCH,
	RUN,
	PRIMARY_ATTACK,
	SECONDARY_ATTACK,
	TERTIARY_ATTACK,
	INTERACTION,
	DODGE,
	OPTIONS,
	PITCH_UP,
	PITCH_DOWN,
	ASCEND,
	DESCEND,
	EXIT,
	RENDERMODE_DEBUG,
	RENDERMODE_DEFAULT,
	MOUSELEFT,
	MOUSERIGHT,
};
//FIXME: Needs event handling for keyup events
////////////////////////////////////////////////////
// Utility class, used for storing button states  //
// between event callbacks.						  //
// Has generic function to allow host program	  //
// to query valid key states as required          //
////////////////////////////////////////////////////
class Controller
{
public:
	Controller(void);
	bool getButtonState(ButtonType buttonQuery){return keyBuffer[buttonQuery];}
	int getMouseMovementX();
	int getMouseMovementY();
	int getMouseWheelRoll(){return mouseWheelturn;}
	void setWindow(SDL_Window* p_Window){m_Window = p_Window;}
	bool update(SDL_Event& p_Event);
	~Controller(void);
private:
	bool keyBuffer[256];
	int mouseX;
	int mouseY;
	int lastMouseX;
	int lastMouseY;
	float mouseWheelturn;
	SDL_Window* m_Window;
};