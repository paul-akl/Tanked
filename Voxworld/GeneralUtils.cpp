#include "GeneralUtils.h"
#include <iostream>
namespace Utils
{
	GeneralUtils::GeneralUtils(void)
	{
		
	}

	void GeneralUtils::exitFatalError(const char *message) 
	{
		std::cout << message << " ";
		std::cin.get();
		exit(1);
	}

	void GeneralUtils::initialiseTTF()
	{
			// set up TrueType / SDL_ttf
		if (TTF_Init()== -1)
			std::cout << "TTF failed to initialise." << std::endl;
			
		//generateFont(textFont, "Fonts/Blox2.ttf");
	}

	TTF_Font* GeneralUtils::generateFont(TTF_Font* font, const char* fileName)
{
	if (font == nullptr)
	{
		font = TTF_OpenFont(fileName, 24);
		std::cout << "opening font: " << fileName << std::endl;
	
		if (font == nullptr)
			std::cout << "Failed to open font." << std::endl;
	}

		return font;
}

	GLuint GeneralUtils::textToTexture(GLuint texId, const char* str, TTF_Font* fontType, glm::vec4 fontColour) {
		
	TTF_Font* font = fontType;
	SDL_Color colour = {(Uint8)fontColour.r, (Uint8)fontColour.g, (Uint8)fontColour.b, (Uint8)fontColour.a};
	SDL_Color bg = { 0, 0, 0 };

	SDL_Surface *stringImage;
	stringImage = TTF_RenderText_Blended(font,str,colour);

	if (stringImage == NULL)
		//exitFatalError("String surface not created.");
		std::cout << "String surface not created." << std::endl;

	GLuint w = stringImage->w;
	GLuint h = stringImage->h;
	GLuint colours = stringImage->format->BytesPerPixel;

	GLuint format, internalFormat;
	if (colours == 4) {   // alpha
		if (stringImage->format->Rmask == 0x000000ff)
			format = GL_RGBA;
	    else
		    format = GL_BGRA;
	} else {             // no alpha
		if (stringImage->format->Rmask == 0x000000ff)
			format = GL_RGB;
	    else
		    format = GL_BGR;
	}
	internalFormat = (colours == 4) ? GL_RGBA : GL_RGB;

	//GLuint texture;
	if(texId == NULL)
	glGenTextures(1, &texId);

	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0,
                    format, GL_UNSIGNED_BYTE, stringImage->pixels);

	// SDL surface was used to generate the texture but is no longer
	// required. Release it to free memory
	SDL_FreeSurface(stringImage);
	return texId;
}

	GeneralUtils::~GeneralUtils(void)
	{
	}
};