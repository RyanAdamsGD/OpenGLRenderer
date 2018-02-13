#include "ParticleSystem.h"
#include "GL\glew.h"
#include <QtOpenGL\qglwidget>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <Qt\qfile.h>
#include <Qt\qtextstream.h>
#include <Qt\qdebug.h>
#include <Qt\qimage.h>
#include "Math Defines.h"
#include "Helper Functions.h"


void ParticleSystem::compileShader(const uint& shaderID, const char* fileName)
{
	std::string file = fileName;
	std::string fileLoc = "Shaders\\" + file;
	QFile input(fileLoc.c_str());
	if(!input.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<< "File failed to open: "<< fileName;
		//exit(1);
	}
	QTextStream stream(&input);
	QString shaderCodeText = stream.readAll();
	input.close();
	std::string temp = shaderCodeText.toStdString();
	const char *shaderSource = temp.c_str();

	const char* buf[1];
	buf[0] = shaderSource;
	glShaderSource(shaderID,1,buf,NULL);
	glCompileShader(shaderID);

	GLint status;
	glGetShaderiv(shaderID,GL_COMPILE_STATUS,&status);
	if(status != GL_TRUE)
	{
		//TODO:
		//Log this
		GLint infoLogLength;
		glGetShaderiv(shaderID,GL_INFO_LOG_LENGTH,&infoLogLength);
		char* buffer = new char[infoLogLength];
		GLsizei bitBucket;
		glGetShaderInfoLog(shaderID,infoLogLength,&bitBucket,buffer);
		std::cout<<buffer;
		delete[] buffer;
		//exit(1);
	}
}

void ParticleSystem::createParticleSystem(const glm::vec3& pos,uint numOfParticles)
{
	drawBuf=0;
	particleCount = numOfParticles;
	position = pos;
	glEnable(GL_PROGRAM_POINT_SIZE);

	createShader();
	fillBuffer();
	createFeedback();
	genVertexArrays();
}


void ParticleSystem::drawParticles(const glm::mat4& projection,const glm::mat4& view,float dt)
{
	if(dt>0.0f)
	{
		runTime += dt;
	}

	glUseProgram(programID);

	draw(projection,view,dt);
}

void ParticleSystem::loadTexture(const char* filename)
{
	int i=0;
	while(filename[i] != '\0')
	{
		i++;
	}
	const char* fileExtension = &filename[i-3];
	QImage timg =
		QGLWidget::convertToGLFormat(QImage(filename,fileExtension));
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
}
