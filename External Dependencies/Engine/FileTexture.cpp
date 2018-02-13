#include "FileTexture.h"
#include <Qt\qimage.h>
#include <GL\glew.h>
#include <QtOpenGL\qglwidget>


FileTexture::FileTexture(const char* fileName)
	:fileName(fileName)
{
	loadTexture();
}


FileTexture::~FileTexture(void)
{
}

void FileTexture::loadTexture()
{	
	QImage image(fileName);
	image.convertToFormat(QImage::Format::Format_ARGB32);
	QImage timg = QGLWidget::convertToGLFormat(image);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timg.width(),
		timg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		timg.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void FileTexture::bindToFrameBuffer()const
{
	glBindFramebuffer(GL_FRAMEBUFFER,textureFrameBufferID);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void FileTexture::bindTexture()const
{
	glBindTexture(GL_TEXTURE_2D,textureID);
}