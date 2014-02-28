#pragma once
#include <GL\glew.h>
#include <SDL_ttf.h>
#include <SDL_blendmode.h>
#include <glm\glm.hpp>
#include <iostream>
#include <sstream>
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
		static GLuint textToTexture(GLuint texId, const char * str, TTF_Font* fontType, glm::vec4 fontColour);
		TTF_Font* getFont(){return textFont;}
		~GeneralUtils(void);
	private:
		 TTF_Font * textFont;
	};
	
	static std::string toString(int p_int)
	{
		std::stringstream stringstream_ret;
		stringstream_ret << p_int;
		return stringstream_ret.str();
	}
	static std::string toString(unsigned int p_uint)
	{
		std::stringstream stringstream_ret;
		stringstream_ret << p_uint;
		return stringstream_ret.str();
	}
	static std::string toString(double p_double)
	{
		std::stringstream stringstream_ret;
		stringstream_ret << p_double;
		return stringstream_ret.str();
	}
	static std::string toString(float p_float)
	{
		std::stringstream stringstream_ret;
		stringstream_ret << p_float;
		return stringstream_ret.str();
	}
	static std::string toString(bool p_bool)
	{
		return (p_bool) ? "true" : "false";
	}
	static std::string toString(char p_char)
	{
		std::string stringRet;
		stringRet += p_char;
		return stringRet;
	}
};
