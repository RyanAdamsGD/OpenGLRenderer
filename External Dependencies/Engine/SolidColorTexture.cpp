#include "SolidColorTexture.h"
#include "GL\glew.h"

SolidColorTexture::SolidColorTexture(const glm::vec4& color)
	:color(color)
{
	createTexture();
}

SolidColorTexture::~SolidColorTexture(void)
{
}

void SolidColorTexture::createTexture()
{
	uint colorValue = colorVecToUint(color);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1,1, 0, GL_RGBA, GL_UNSIGNED_BYTE,&colorValue);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void SolidColorTexture::bindToFrameBuffer()const
{
	glBindFramebuffer(GL_FRAMEBUFFER,textureFrameBufferID);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void SolidColorTexture::bindTexture()const
{
	glBindTexture(GL_TEXTURE_2D,textureID);
}

void SolidColorTexture::setColor(const glm::vec4& value)
{
	uint colorValue = colorVecToUint(color);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1,1, 0, GL_RGBA, GL_UNSIGNED_BYTE,&colorValue);
	glBindTexture(GL_TEXTURE_2D, 0);
}

uint SolidColorTexture::colorVecToUint(const glm::vec4& color)
{
	uint colorValue = 0;
	glm::clamp(color,glm::vec4(0.0f),glm::vec4(1.0f));
	colorValue += color.r * 255;
	colorValue = colorValue << 8;
	colorValue += color.b * 255;
	colorValue = colorValue << 8;
	colorValue += color.g * 255;
	colorValue = colorValue << 8;
	colorValue += color.a * 255;

	return colorValue;
}