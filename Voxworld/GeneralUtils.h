#pragma once
#include <GL\glew.h>
#include <SDL_ttf.h>
#include <SDL_blendmode.h>
#include <glm\glm.hpp>
namespace Utils
{
	class GeneralUtils
	{
	public:
		GeneralUtils(void);
		static void exitFatalError(const char *message);

		template <class T> static const T& max (const T& a, const T& b) {
		 return (a<b)?b:a;    
		}

		static bool timerElapsed(int currentTimer, int duration)
		{
			if (currentTimer >= duration)
				return true;
			return false;
		}

		void initialiseTTF();
		static TTF_Font* generateFont(TTF_Font* font, const char* fileName);
		static GLuint textToTexture(const char * str, TTF_Font* fontType, glm::vec4 fontColour);
		TTF_Font* getFont(){return textFont;}
		~GeneralUtils(void);
	private:
		 TTF_Font * textFont;
	};
};
