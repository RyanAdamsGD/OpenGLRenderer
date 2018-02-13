#ifndef SUBROUTINE_UNIFORM_MANAGER
#define SUBROUTINE_UNIFORM_MANAGER
#include "MyTypeDefs.h"

class SubroutineUniformCallback
{
	uint programID;
	
	//array of pointers to subroutine uniform names
	const char* const** subroutineUniformNames;
	
	//holds indicies of both vertex and fragment shaders
	uint* subroutineIndices;
	//allows us to index correctly when activating subroutines
	uint numOfSubroutines;
	uint offsetFragmentSubroutines;

	void activateSubroutines(uint count, uint* indices, uint shaderType)const;
	bool findSubroutine(const char* subroutineType, const char* const* subroutineUniformName);
public:
	SubroutineUniformCallback(uint programID);
	~SubroutineUniformCallback(void);

	void addSubroutine(const char* const* subroutineUniformName, const char* subroutineType);
	void activateUniforms()const;
	void updateUniform(const char* const* subroutineUniformName, const char* subroutineType);
};

#endif