#include "SubroutineUniformCallback.h"
#include "GL\glew.h"
#include "Logger.h"
#include <string>

SubroutineUniformCallback::SubroutineUniformCallback(uint programID)
	:subroutineIndices(0),subroutineUniformNames(0),programID(programID)
{
	GLint numActiveSubUni;
	glGetProgramStageiv(programID, GL_VERTEX_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &numActiveSubUni);
	offsetFragmentSubroutines = numActiveSubUni;
	glGetProgramStageiv(programID, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &numActiveSubUni);
	numOfSubroutines = offsetFragmentSubroutines + numActiveSubUni;

	subroutineIndices = new uint[numOfSubroutines];
	subroutineUniformNames = new const char* const*[numOfSubroutines];
	for(uint i=0;i<numOfSubroutines;i++)
	{
		subroutineIndices[i] = -1;
		subroutineUniformNames[i] = 0;
	}
}

SubroutineUniformCallback::~SubroutineUniformCallback(void)
{
}

void SubroutineUniformCallback::addSubroutine(const char* const* subroutineUniformName, const char* subroutineType)
{
	bool subroutineFound = findSubroutine(subroutineType,subroutineUniformName);
#ifdef USE_LOGGER
	if(!subroutineFound)
	{
		std::string msg = "Could not find subroutine: ";
		msg += *subroutineUniformName;
		LOGW(msg.c_str());
	}
#endif
}

bool SubroutineUniformCallback::findSubroutine(const char* subroutineType, const char* const* subroutineUniformName)
{
	//parse through the subroutine uniforms until we find one with the matching type
	//once we do store the data into the appropriate index in uniformNames
	char name[256];
	int length;
	GLint numSubroutines;
	for(uint i=0;i<offsetFragmentSubroutines;i++)
	{
		glGetActiveSubroutineUniformName(programID, GL_VERTEX_SHADER, i, 256, &length, name);
		if(strcmp(name, subroutineType) == 0)
		{
			subroutineUniformNames[glGetSubroutineUniformLocation(programID,GL_FRAGMENT_SHADER,name)] = subroutineUniformName;
			return true;
		}
	}
	for(uint i=0;i<numOfSubroutines - offsetFragmentSubroutines;i++)
	{
		glGetActiveSubroutineUniformName(programID, GL_FRAGMENT_SHADER, i, 256, &length, name);
		if(strcmp(name, subroutineType) == 0)
		{
			subroutineUniformNames[glGetSubroutineUniformLocation(programID,GL_FRAGMENT_SHADER,name)] = subroutineUniformName;
			return true;
		}
	}
	return false;
}

void SubroutineUniformCallback::activateUniforms()const
{
	for(uint i=0;i<numOfSubroutines;i++)
	{
		//dont try to activate uniforms that aren't set
		if(subroutineUniformNames[i] == 0)
			continue;

		if(i<offsetFragmentSubroutines)
			subroutineIndices[i] = glGetSubroutineIndex(programID,GL_VERTEX_SHADER,*subroutineUniformNames[i]);
		else
			subroutineIndices[i] = glGetSubroutineIndex(programID,GL_FRAGMENT_SHADER,*subroutineUniformNames[i]);
	}
	activateSubroutines(offsetFragmentSubroutines,subroutineIndices,GL_VERTEX_SHADER);
	activateSubroutines(numOfSubroutines - offsetFragmentSubroutines,&subroutineIndices[offsetFragmentSubroutines],GL_FRAGMENT_SHADER);
}

void SubroutineUniformCallback::activateSubroutines(uint count, uint* indices, uint shaderType)const
{
	glUniformSubroutinesuiv(shaderType, count, indices);
}

void SubroutineUniformCallback::updateUniform(const char* const* subroutineUniformName, const char* subroutineType)
{
	bool subroutineFound = findSubroutine(subroutineType,subroutineUniformName);
#ifdef USE_LOGGER
	if(!subroutineFound)
	{
		std::string msg = "Could not find subroutine: ";
		msg += *subroutineType;
		LOGW(msg.c_str());
	}
#endif
}