#include "stdafx.h"
#include <SOIL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include "Texture2D.h"

unsigned int GLFW::Texture2D::numberOfUsed = 0;
const int GLFW::Texture2D::TEXTURE_DISABLED = -1;

void GLFW::Texture2D::use(bool flag)
{
	if (flag)
	{
		/*GLFW::Texture2D::numberOfUsed++;
		glActiveTexture(GL_TEXTURE0 + GLFW::Texture2D::numberOfUsed);
		glBindTexture(GL_TEXTURE_2D, this->id);*/
		
		if (this->activationId == GLFW::Texture2D::TEXTURE_DISABLED)
		{
			this->activationId = GL_TEXTURE0 + 
				GLFW::Texture2D::numberOfUsed;
			GLFW::Texture2D::numberOfUsed++;
		}
		glActiveTexture(this->activationId);
		glBindTexture(GL_TEXTURE_2D, this->id);
	}
	else
	{
		/*glActiveTexture(GL_TEXTURE0 + GLFW::Texture2D::numberOfUsed);
		glBindTexture(GL_TEXTURE_2D, 0);
		GLFW::Texture2D::numberOfUsed--;*/
		glActiveTexture(this->activationId);
		glBindTexture(GL_TEXTURE_2D, 0);
		this->activationId = GLFW::Texture2D::TEXTURE_DISABLED;
	}
}

void GLFW::Texture2D::deserialize(std::string & s)
{
	const unsigned char* imgSrc = 
		reinterpret_cast<const unsigned char *>(s.c_str());

	this->use();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/*SOIL_load_OGL_texture_from_memory
			(
				imgSrc,
				s.length(),
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT
			);*/
	this->pixels = SOIL_load_image_from_memory
					(
						imgSrc,
						s.length(),
						&this->width,
						&this->height,
						&this->channels,
						SOIL_LOAD_AUTO 
					);
	
	if (this->pixels == 0)
	{
#ifdef _DEBUG
		std::cerr << "Texture2D: Texture data is corrupted or empty..\n";
		return;
#endif
	}

	invertY();
	int tmp = GL_RGBA;
	if (channels == 3)
		tmp = GL_RGB;
	
	glTexImage2D
		(
			GL_TEXTURE_2D, 0, 
			tmp, 
			this->width, 
			this->height, 0, 
			tmp, 
			GL_UNSIGNED_BYTE, 
			this->pixels
		);
	glGenerateMipmap(GL_TEXTURE_2D);
	this->use(false);
	SOIL_free_image_data(pixels);
}

void GLFW::Texture2D::invertY()
{
	int i, j;
	for (j = 0; j * 2 < height; ++j)
	{
		int index1 = j * width * channels;
		int index2 = (height - 1 - j) * width * channels;
		for (i = width * channels; i > 0; --i)
		{
			unsigned char temp = pixels[index1];
			pixels[index1] = pixels[index2];
			pixels[index2] = temp;
			++index1;
			++index2;
		}
	}
}

GLFW::Texture2D::Texture2D() : Resource()
{
	GLuint texId;
	glGenTextures(1, &texId);
	this->id = texId;
}


GLFW::Texture2D::~Texture2D()
{
	GLuint texId = this->id;
	glDeleteTextures(1, &texId);
}
