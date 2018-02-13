#include "ShaderUniformManager.h"
#include "GL\glew.h"
#include "ShaderManager.h"

uint ShaderUniformManager::currentlyBoundTextureCount = 0;

ShaderUniformManager::ShaderUniformManager(uint shaderManagerID)
	:programID(Shader_Manager.getProgramID(shaderManagerID)),subroutineCallback(Shader_Manager.getProgramID(shaderManagerID)),uniformBlocks(Shader_Manager.getProgramID(shaderManagerID))
{
}

ShaderUniformManager::~ShaderUniformManager()
{
	uniformCallbackInfos.clear();
}

void ShaderUniformManager::activateUniforms()const
{
	currentlyBoundTextureCount = 0;
	uint size = uniformCallbackInfos.size();
	for(uint i=0;i<size;i++)
	{
		uniformCallbackInfos[i].activateUniform();
	}
	subroutineCallback.activateUniforms();
}

void ShaderUniformManager::addUniform(const void* data, const char* variableName,const DataType& type)
{
	if(type != SUBROUTINE)
		uniformCallbackInfos.push_back(UniformCallbackInfo(programID,data,variableName,type));
	else
		subroutineCallback.addSubroutine(reinterpret_cast<const char* const*>(data),variableName);
}

void ShaderUniformManager::removeUniform(const void* data)
{
	uint size = uniformCallbackInfos.size();
	for(uint i=0;i<size;i++)
	{
		UniformCallbackInfo& uniformCallbackInfo = uniformCallbackInfos[i];
		if(uniformCallbackInfo.getData() == data)
		{
			uniformCallbackInfos.erase(uniformCallbackInfos.begin() + i);
			size--;
			i--;
		}
	}
}

void ShaderUniformManager::removeUniform(const char* variableName)
{
	uint size = uniformCallbackInfos.size();
	for(uint i=0;i<size;i++)
	{
		UniformCallbackInfo& uniformCallbackInfo = uniformCallbackInfos[i];
		if(strcmp(uniformCallbackInfo.getVariableName(),variableName) == 0)
		{
			uniformCallbackInfos.erase(uniformCallbackInfos.begin() + i);
			size--;
			i--;
		}
	}
}

void ShaderUniformManager::updateUniform(const void* data, const char* variableName, const DataType& type)
{
	if(type == SUBROUTINE)
		subroutineCallback.updateUniform(reinterpret_cast<const char* const*>(data),variableName);
	else
	{
		uint size = uniformCallbackInfos.size();
		for(uint i=0;i<size;i++)
		{
			UniformCallbackInfo& uniformCallbackInfo = uniformCallbackInfos[i];
			if(strcmp(uniformCallbackInfo.getVariableName(),variableName) == 0)
			{
				uniformCallbackInfos[i].setData(data);
			}
		}
	}
}

uint ShaderUniformManager::addUniformBlock(const char* uniformBlockName, const char* uniformStructName)
{
	return uniformBlocks.addUniformBlock(uniformBlockName,uniformStructName);
}

uint ShaderUniformManager::getUniformBlockID(const char* variableName)const
{
	return uniformBlocks.getUniformBlockID(variableName);
}