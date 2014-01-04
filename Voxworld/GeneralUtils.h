#pragma once
#include <GL\glew.h>
#include <SDL_ttf.h>
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

		void initialiseFont();

		GLuint textToTexture(const char * str/*, TTF_Font *font, SDL_Color colour, GLuint &w,GLuint &h */);

		~GeneralUtils(void);
	//private:
		TTF_Font * textFont;
	};
};
