#include "ShaderManager.h"
#include <string>
#include <GL\glew.h>
#include <Qt\qfile.h>
#include <Qt\qtextstream.h>
#include "Logger.h"
#include "LuaState.h"


ShaderManager ShaderManager::instance;

ShaderManager::ShaderManager(void)
	:defaultShaderProgram(-1),shaderFileLocation("")
{
	
}

ShaderManager::~ShaderManager(void)
{
}

int ShaderManager::checkForShader(const char* fileName, const std::vector<ShaderInfo>& shaders)
{
	uint size = shaders.size();
	for(uint i=0;i<size;i++)
	{
		if(strcmp(fileName,shaders[i].fileName) == 0)
			return i;
	}
	return -1;
}

ShaderManager::ShaderInfo::ShaderInfo(const char* fileName, uint type)
	:fileName(fileName)
{
	shaderID = glCreateShader(type);
	uint err = glGetError();
	std::string file = fileName;
	std::string fileLoc = Shader_Manager.shaderFileLocation + file;
	QFile input(fileLoc.c_str());
#ifdef USE_LOGGER
	if(!input.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		const char* error = (std::string("File failed to open: ") + fileName).c_str();
		LOGS(error);
	}
#endif

	QTextStream stream(&input);
	QString shaderCodeText = stream.readAll();
	input.close();
	std::string temp = shaderCodeText.toStdString();
	const char *shaderSource = temp.c_str();

	glShaderSource(shaderID,1,&shaderSource,NULL);
	glCompileShader(shaderID);
	err = glGetError();
	


#ifdef USE_LOGGER
	GLint status;
	glGetShaderiv(shaderID,GL_COMPILE_STATUS,&status);
	if(status != GL_TRUE)
	{
		GLint infoLogLength;
		glGetShaderiv(shaderID,GL_INFO_LOG_LENGTH,&infoLogLength);
		char* buffer = (char*) malloc(infoLogLength);
		GLsizei bitBucket;
		glGetShaderInfoLog(shaderID,infoLogLength,&bitBucket,buffer);
		std::string strError("Failed to compile shader: ");
		strError += fileName;
		const char* error = strError.c_str();
		LOGS(error);
		LOGS(buffer);
		free(buffer);
	}
#endif
}

uint ShaderManager::addProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName)
{
	//create the program
	uint programID = glCreateProgram();

	//make the vertex shader if we need to
	int shaderIndex = checkForShader(vertexShaderFileName, vertexShaders);
	if(shaderIndex < 0)
	{
		ShaderInfo vertexShader(vertexShaderFileName, GL_VERTEX_SHADER);
		shaderIndex = vertexShaders.size();
		vertexShaders.push_back(vertexShader);
	}
	glAttachShader(programID,vertexShaders[shaderIndex].shaderID);

	//make the fragment shader if we need to
	shaderIndex = checkForShader(fragmentShaderFileName, fragmentShaders);
	if(shaderIndex < 0)
	{
		ShaderInfo fragmentShader(fragmentShaderFileName, GL_FRAGMENT_SHADER);
		shaderIndex = fragmentShaders.size();
		fragmentShaders.push_back(fragmentShader);
	}
	glAttachShader(programID,fragmentShaders[shaderIndex].shaderID);

	glLinkProgram(programID);
	uint err = glGetError();
	shaderProgramIDs.push_back(programID);
	return shaderProgramIDs.size() -1;
}

uint ShaderManager::getProgramID(uint index)
{
	if(index > totalShaderProgramCount)
	{
#ifdef USE_LOGGER
		char message[70];
		sprintf(message,"Requested program index: %u is out of range.", index);
		LOGW(message);
#endif
		return NULL;
	}

	return shaderProgramIDs[index];
}

uint ShaderManager::getDefault()
{
	//check to see if the default program has been set up yet
	if(defaultShaderProgram < 0)
	{
		LuaState luaState("../../External Dependencies/Engine/config.lua");
		defaultShaderProgram = addProgram(luaState.get<const char*>("DefaultVertexShader"),luaState.get<const char*>("DefaultFragmentShader"));
	}
	return defaultShaderProgram;
}