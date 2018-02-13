#include "Texture.h"
#include <GL\glew.h>
#include <Qt\qimage.h>

uint Texture::textureFrameBufferID = 0;

Texture::Texture(uint textureID,uint width, uint height) 
	:textureID(textureID),width(width),height(height)
{

}

Texture::Texture(void) 
	:textureID(0),width(0),height(0)
{
}

Texture::Texture(const Texture& rhs)
	:textureID(rhs.textureID),width(rhs.width),height(rhs.height)
{
	rhs.textureID = 0;
}

Texture& Texture::operator =(const Texture& rhs)
{
	this->textureID = rhs.textureID;
	this->width = rhs.width;
	this->height = rhs.height;

	rhs.textureID = 0;
	return *this;
}

Texture::~Texture(void)
{
	if(textureID != 0)
		glDeleteTextures(1,&textureID);

	textureID = 0;
}

void Texture::setTextureID(uint value)
{
	textureID = value;
}


void Texture::updateTexture(const fastdelegate::FastDelegate2<const glm::mat4&,const glm::mat4&>& drawFunction)const
{

}

bool Texture::saveToFile(const char* fileName)const
{
	if(height == 0 || width == 0)
		return false;

	float* texRead = new float[width*height*4];

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,textureID);
	glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,texRead);

	uchar* rgbaTex = new uchar[width*height*4];

	for(int w=0; w<width; w++)
		for(int h=0; h<height; h++)
		{
			float f = texRead[4*(height*w + h)] * 255.0f;
			uchar* rgba = rgbaTex + 4*(height*w + h);
			rgba[0] = (uchar)(texRead[4*(height*w + h)] * 255.0f);
			rgba[1] = (uchar)(texRead[4*(height*w + h) +1] * 255.0f);
			rgba[2] = (uchar)(texRead[4*(height*w + h) +2] * 255.0f);
			rgba[3] = (uchar)(texRead[4*(height*w + h) +3] * 255.0f);
		}

		QImage qimg(rgbaTex, width, height, QImage::Format_ARGB32);
		bool success = qimg.save(fileName);

		delete [] texRead;
		delete [] rgbaTex;
		return success;;
}