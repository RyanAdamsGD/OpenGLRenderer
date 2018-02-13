#include "CubeMapTexture.h"
#include <GL\glew.h>
#include "DepthTexture.h"

CubeMapTexture::CubeMapTexture(uint texelCount)
	:Texture(0,texelCount,texelCount)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//create room for all 6 of the face textures
	for(uint i=0;i<6;i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA, texelCount, texelCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


CubeMapTexture::~CubeMapTexture(void)
{
}

void CubeMapTexture::updateTexture(const fastdelegate::FastDelegate2<const glm::mat4&,const glm::mat4&>& drawFunction)
{
	if(staticTexture)
		return;

	//we better be rendering square images
	assert(width == height);

	glBindFramebuffer(GL_FRAMEBUFFER,textureFrameBufferID);
	depthTexture->bindToFrameBuffer();
	glViewport(0,0,width,height);
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glm::mat4 cubeProjection = glm::perspective(90.0f,1.0f,2.1f,100.0f);
	glm::mat4 cubeView;
	for(uint i=0;i<6;i++)
	{
		//attach the next texture and clear its old data
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureID, 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		switch(i)
		{
		case 1:
			cubeView = glm::lookAt(glm::vec3(),glm::vec3(-1.0f,0.0f,0.0f),glm::vec3(0.0f,-1.0f,0.0f));
			break;
		case 2:
			cubeView = glm::lookAt(glm::vec3(),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f));
			break;
		case 3:
			cubeView = glm::lookAt(glm::vec3(),glm::vec3(0.0f,-1.0f,0.0f),glm::vec3(0.0f,0.0f,-1.0f));
			break;
		case 4:
			cubeView = glm::lookAt(glm::vec3(),glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.0f,-1.0f,0.0f));
			break;
		case 5:
			cubeView = glm::lookAt(glm::vec3(),glm::vec3(0.0f,0.0f,-1.0f),glm::vec3(0.0f,-1.0f,0.0f));
			break;
		default:
			cubeView = glm::lookAt(glm::vec3(),glm::vec3(1.0f,0.0f,0.0f),glm::vec3(0.0f,-1.0f,0.0f));
			break;
		}
		drawFunction(cubeView,cubeProjection);
	}
}

//TODO: figure out how to implement this
bool CubeMapTexture::saveToFile(const char* fileName) const
{
	return false;
}

void CubeMapTexture::setWidth(uint value) 
{
	updateTexelCount(value);
}

void CubeMapTexture::setHeight(uint value)
{
	updateTexelCount(value);
}

void CubeMapTexture::updateTexelCount(uint texelCount)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for(uint i=0;i<6;i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA, texelCount, texelCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMapTexture::bindTexture()const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP,textureID);
}