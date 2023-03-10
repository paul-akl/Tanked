#include <exception>
#include <FreeImage.h>
#include <iostream>
#include <SDL.h>
#include "Renderer.h"
#include "TextureNode.h"

TextureNode::TextureNode(void)
{
	m_Texture = NULL;
}

bool TextureNode::loadTexture(const std::string& p_FileName)
{
	FREE_IMAGE_FORMAT v_ImageFormat = FreeImage_GetFileType(p_FileName.c_str(), 0);	// Get the format of the image
	FIBITMAP* v_Bitmap = FreeImage_Load(v_ImageFormat, p_FileName.c_str());			// Reading the actual image

	v_Bitmap = FreeImage_ConvertTo32Bits(v_Bitmap);		// Conversion to internat format
		
	int v_imageWidth = FreeImage_GetWidth(v_Bitmap);	// Get image width
	int v_imageHeight = FreeImage_GetHeight(v_Bitmap);	// and height

	if(v_Bitmap)	// Check if image was loaded
	{		
		GLubyte* v_Texture2D = new GLubyte[4*v_imageWidth*v_imageHeight];	// Variables required for converting to RGBA format
		char* v_Pixels = (char*)FreeImage_GetBits(v_Bitmap);					
	
		//FreeImage loads in BGR format, therefore swap of bytes is needed (Or usage of GL_BGR).
		for(int i = 0; i < v_imageWidth * v_imageHeight; i++)	// For every pixel on the image
		{												// From	 |BGRA|  to	  |RGBA|
			v_Texture2D[i * 4 + 0] = v_Pixels[i * 4 + 2];	// Put z value into x (RED	 color component)
			v_Texture2D[i * 4 + 1] = v_Pixels[i * 4 + 1];	// Put y value into y (GREEN color component)
			v_Texture2D[i * 4 + 2] = v_Pixels[i * 4 + 0];	// Put x value into z (BLUE	 color component)
			v_Texture2D[i * 4 + 3] = v_Pixels[i * 4 + 3];	// Put w value into w (ALPHA color component)
		}
	
		//Generating the actual OpenGL Texture2D
		glGenTextures(1, &m_Texture);											// Generate texture ID
		glBindTexture(GL_TEXTURE_2D, m_Texture);								// Bind it, for changes below
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, v_imageWidth, v_imageHeight,	// Upload image to OpenGL
					 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)v_Texture2D );		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// Texture2D filtering mode, when image is minimized, Linear with mipmaps for best quality
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);				// Texture2D filtering mode, when image is magnified
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);			// Set image clamping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		GLfloat v_TextureAnisFilter;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &v_TextureAnisFilter);					// Get the maximum anisotropic filtering available from current hardware
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, v_TextureAnisFilter);		// Set to maximum anisitropic filtering, for best quality
	}
	else	// If image wasn't loaded, throw an exception
	{
		std::cout << "Error loading texture: " << p_FileName << std::endl;	// Delete this line, once catching of the exception has been implemented
		throw new std::exception(("Error loading texture: " + p_FileName).c_str());
	}
	return true;
}

void TextureNode::render(Renderer* p_Renderer)
{
	p_Renderer->render(this);
}
TextureNode::~TextureNode(void)
{
	
}
