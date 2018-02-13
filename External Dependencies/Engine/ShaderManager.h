#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H
#include "ExportHeader.h"
#include "MyTypeDefs.h"
#include <vector>

class ENGINE_SHARED ShaderManager
{
	std::vector<uint> shaderProgramIDs;
	uint totalShaderProgramCount;
	struct ShaderInfo
	{
		const char* fileName;
		uint shaderID;
		ShaderInfo(const char* fileName,uint type);
	};

	std::vector<ShaderInfo> vertexShaders;
	std::vector<ShaderInfo> fragmentShaders;

	int checkForShader(const char* fileName, const std::vector<ShaderInfo>& shaders);

	friend class ShaderInfo;
	char* shaderFileLocation;

	friend class ShaderUniformManager;
	uint addProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName);

	int defaultShaderProgram;

	static ShaderManager instance;
	ShaderManager(void);
	ShaderManager(const ShaderManager&);
	ShaderManager& operator =(const ShaderManager&);
public:
	~ShaderManager(void);

	inline static ShaderManager& getInstance() { return instance; }

	uint getProgramID(uint index);
	uint getDefault();
};

#define Shader_Manager ShaderManager::getInstance()

#endif