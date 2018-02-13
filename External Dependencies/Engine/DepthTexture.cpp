#include "DepthTexture.h"
#include <Qt\qimage.h>
#include <GL\glew.h>

DepthTexture::DepthTexture(uint width, uint height)
	:Texture(0,width,height)
{
	glGenRenderbuffers(1, &textureID);
	glBindRenderbuffer(GL_RENDERBUFFER, textureID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

DepthTexture::~DepthTexture(void)
{
}


bool DepthTexture::saveToFile(const char* fileName) const
{
	if(height == 0 || width == 0)
		return false;

	float* texRead = new float[width*height];

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,textureID);
	glGetTexImage(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,GL_FLOAT,texRead);

	uchar* rgbaTex = new uchar[width*height*4];

	for(int w=0; w<width; w++)
		for(int h=0; h<height; h++)
		{
			float floatColor = texRead[height*w + h] * 255.0f;
			uchar* rgba = rgbaTex + 4*(height*w + h);
			uchar color = (uchar)floatColor;
			rgba[0] = color;
			rgba[1] = color;
			rgba[2] = color;
			rgba[3] = 255;
		}


	QImage qimg(rgbaTex, width, height, QImage::Format_ARGB32);
	bool success = qimg.save(fileName);

	delete [] texRead;
	delete [] rgbaTex;
	return success;;
}

void DepthTexture::bindToFrameBuffer()const
{
	glBindFramebuffer(GL_FRAMEBUFFER,textureFrameBufferID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, textureID);
}

void DepthTexture::setWidth(uint value)
{
	width = value;
	glBindRenderbuffer(GL_RENDERBUFFER, textureID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void DepthTexture::setHeight(uint value)
{
	height = value;
	glBindRenderbuffer(GL_RENDERBUFFER, textureID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void DepthTexture::bindTexture()const
{
	glBindTexture(GL_TEXTURE_2D,textureID);
}
