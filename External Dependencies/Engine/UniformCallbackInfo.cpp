#include "UniformCallbackInfo.h"
#include "GL\glew.h"
#include "ShaderUniformManager.h"
#include "Texture.h"
#include "TextureManager.h"
#include "Logger.h"

//set up the static uniform callbacks
glUniformCallback UniformCallbackInfo::glUniformCallbacks[NUM_UNIFORM_CALLBACKS] =
{
	UniformCallbackInfo::uniformCallback1i,
	UniformCallbackInfo::uniformCallback2i,
	UniformCallbackInfo::uniformCallback3i,
	UniformCallbackInfo::uniformCallback4i,
	UniformCallbackInfo::uniformCallback1ui,
	UniformCallbackInfo::uniformCallback2ui,
	UniformCallbackInfo::uniformCallback3ui,
	UniformCallbackInfo::uniformCallback4ui,
	UniformCallbackInfo::uniformCallback1f,
	UniformCallbackInfo::uniformCallback2f,
	UniformCallbackInfo::uniformCallback3f,
	UniformCallbackInfo::uniformCallback4f,
	UniformCallbackInfo::uniformCallback2m,
	UniformCallbackInfo::uniformCallback3m,
	UniformCallbackInfo::uniformCallback4m,
	UniformCallbackInfo::uniformCallbackTexture
};

void UniformCallbackInfo::activateUniform() const
{
	//use a array of function callbacks since it's faster 
	//than a switch and we will be calling this multiple times
	//per a draw call
	glUniformCallbacks[type](this);
}

UniformCallbackInfo::UniformCallbackInfo(uint programID,  const void* data, const char* variableName, const DataType& type)
	:data(data),location(0),type(type),programID(programID),variableName(variableName)
{
	//get the location in the shader so we don't have to store the string
	location = glGetUniformLocation(programID,variableName);
}

#pragma region GL_Uniform to Callback functions

void UniformCallbackInfo::uniformCallback1i(const UniformCallbackInfo* uniformCallback)
{
	glUniform1iv(uniformCallback->location,1,reinterpret_cast<const int*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallback2i(const UniformCallbackInfo* uniformCallback)
{
	glUniform2iv(uniformCallback->location,1,reinterpret_cast<const int*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallback3i(const UniformCallbackInfo* uniformCallback)
{
	glUniform3iv(uniformCallback->location,1,reinterpret_cast<const int*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallback4i(const UniformCallbackInfo* uniformCallback)
{
	glUniform4iv(uniformCallback->location,1,reinterpret_cast<const int*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallback1ui(const UniformCallbackInfo* uniformCallback)
{
	glUniform1uiv(uniformCallback->location,1,reinterpret_cast<const unsigned int*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallback2ui(const UniformCallbackInfo* uniformCallback)
{
	glUniform2uiv(uniformCallback->location,1,reinterpret_cast<const unsigned int*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallback3ui(const UniformCallbackInfo* uniformCallback)
{
	glUniform3uiv(uniformCallback->location,1,reinterpret_cast<const unsigned int*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallback4ui(const UniformCallbackInfo* uniformCallback)
{
	glUniform4uiv(uniformCallback->location,1,reinterpret_cast<const unsigned int*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallback1f(const UniformCallbackInfo* uniformCallback)
{
	glUniform1fv(uniformCallback->location,1,reinterpret_cast<const float*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallback2f(const UniformCallbackInfo* uniformCallback)
{
	glUniform2fv(uniformCallback->location,1,reinterpret_cast<const float*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallback3f(const UniformCallbackInfo* uniformCallback)
{
	glUniform3fv(uniformCallback->location,1,reinterpret_cast<const float*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallback4f(const UniformCallbackInfo* uniformCallback)
{
	glUniform4fv(uniformCallback->location,1,reinterpret_cast<const float*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallback2m(const UniformCallbackInfo* uniformCallback)
{
	glUniformMatrix2fv(uniformCallback->location,1,GL_FALSE,reinterpret_cast<const float*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallback3m(const UniformCallbackInfo* uniformCallback)
{
	glUniformMatrix3fv(uniformCallback->location,1,GL_FALSE,reinterpret_cast<const float*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallback4m(const UniformCallbackInfo* uniformCallback)
{
	glUniformMatrix4fv(uniformCallback->location,1,GL_FALSE,reinterpret_cast<const float*>(uniformCallback->data));
}

void UniformCallbackInfo::uniformCallbackTexture(const UniformCallbackInfo* uniformCallback)
{
	//check to see if the data already points to an unbound texture
	if(*reinterpret_cast<const uint*>(uniformCallback->data) == UNBOUND_TEXTURE)
		return;
	//get the texture from the manager
	Texture* texture = Texture_Manager.getTexture(*reinterpret_cast<const uint*>(uniformCallback->data));
	//return if there was no texture
	if(texture == NULL)
		return;

	//since open gl requires the approprieate texture be activated first
	//we use the count saved in the manager to figure out which one to activate
	glActiveTexture(GL_TEXTURE0 + ShaderUniformManager::currentlyBoundTextureCount);

	texture->bindTexture();
	glUniform1i(uniformCallback->location,ShaderUniformManager::currentlyBoundTextureCount);
	//update the number of textures used so far
	ShaderUniformManager::currentlyBoundTextureCount++;
}

#pragma endregion
