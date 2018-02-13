#ifndef SHADER_UNIFORM_MANAGER
#define SHADER_UNIFORM_MANAGER
#include "MyTypeDefs.h"
#include "UniformCallbackInfo.h"
#include "SubroutineUniformCallback.h"
#include "UniformBlockManager.h"
#include <vector>

class ShaderUniformManager
{
	uint programID;
	//required to keep track of active texture
	friend class UniformCallbackInfo;
	static uint currentlyBoundTextureCount;
	std::vector<UniformCallbackInfo> uniformCallbackInfos;
	SubroutineUniformCallback subroutineCallback;
	UniformBlockManager uniformBlocks;

public:
	///Textures should pass in the index for the texture manager
	void addUniform(const void* data, const char* variableName,const DataType& type);
	uint addUniformBlock(const char* uniformBlockName, const char* uniformStructName);
	///removes all uniforms with a matching programID and data pointer
	void removeUniform(const void* data);
	void removeUniform(const char* variableName);
	void updateUniform(const void* newData, const char* variableName, const DataType& type);
	void activateUniforms() const;

	uint getUniformBlockID(const char* variableName)const;

	ShaderUniformManager(uint shaderManagerID);
	~ShaderUniformManager(void);

	uint getProgramID() const { return programID; }

#pragma region Template Functions Declarations
	template<class p1>
	uint addDataToUniformBlock1(uint index,const p1& param);

	template <class p1,class p2>
	uint addDataToUniformBlock2(uint index, const p1& param1,const p2& param2);

	template <class p1,class p2, class p3>
	uint addDataToUniformBlock3(uint index, const p1& param1,const p2& param2,const p3& param3);

	template <class p1,class p2, class p3, class p4>
	uint addDataToUniformBlock4(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4);

	template <class p1,class p2, class p3, class p4, class p5>
	uint addDataToUniformBlock5(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5);

	template <class p1,class p2, class p3, class p4, class p5, class p6>
	uint addDataToUniformBlock6(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6);

	template <class p1,class p2, class p3, class p4, class p5, class p6, class p7>
	uint addDataToUniformBlock7(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7);

	template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8>
	uint addDataToUniformBlock8(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
		const p8& param8);

	template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9>
	uint addDataToUniformBlock9(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
		const p8& param8,const p9& param9);

	template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10>
	uint addDataToUniformBlock10(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
		const p8& param8,const p9& param9,const p10& param10);

	template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10, class p11>
	uint addDataToUniformBlock11(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
		const p8& param8,const p9& param9,const p10& param10,const p11& param11);

	template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10, class p11, class p12>
	uint addDataToUniformBlock12(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8,const p9& param9,const p10& param10,const p11& param11,const p12& param12);

#pragma endregion
};

#pragma region Template Function Definitions

template<class p1>
uint ShaderUniformManager::addDataToUniformBlock1(uint index,const p1& param1)
{
	return uniformBlocks.addDataToUniformBlock1<p1>(index,param1);
}

template <class p1,class p2>
uint ShaderUniformManager::addDataToUniformBlock2(uint index, const p1& param1,const p2& param2)
{
	return uniformBlocks.addDataToUniformBlock2<p1,p2>(index,param1,param2);
}

template <class p1,class p2, class p3>
uint ShaderUniformManager::addDataToUniformBlock3(uint index, const p1& param1,const p2& param2,const p3& param3)
{
	return uniformBlocks.addDataToUniformBlock3<p1,p2,p3>(index,param1,param2,param3);
}

template <class p1,class p2, class p3, class p4>
uint ShaderUniformManager::addDataToUniformBlock4(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4)
{
	return uniformBlocks.addDataToUniformBlock4<p1,p2,p3,p4>(index,param1,param2,param3,param4);
}

template <class p1,class p2, class p3, class p4, class p5>
uint ShaderUniformManager::addDataToUniformBlock5(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5)
{
	return uniformBlocks.addDataToUniformBlock5<p1,p2,p3,p4,p5>
		(index,param1,param2,param3,param4,param5);
}

template <class p1,class p2, class p3, class p4, class p5, class p6>
uint ShaderUniformManager::addDataToUniformBlock6(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6)
{
	return uniformBlocks.addDataToUniformBlock6<p1,p2,p3,p4,p5,p6>
		(index,param1,param2,param3,param4,param5,param6);
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7>
uint ShaderUniformManager::addDataToUniformBlock7(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7)
{
	return uniformBlocks.addDataToUniformBlock7<p1,p2,p3,p4,p5,p6,p7>
		(index,param1,param2,param3,param4,param5,param6,param7);
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8>
uint ShaderUniformManager::addDataToUniformBlock8(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8)
{
	return uniformBlocks.addDataToUniformBlock8<p1,p2,p3,p4,p5,p6,p7,p8>
		(index,param1,param2,param3,param4,param5,param6,param7,param8);
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9>
uint ShaderUniformManager::addDataToUniformBlock9(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8,const p9& param9)
{
	return uniformBlocks.addDataToUniformBlock9<p1,p2,p3,p4,p5,p6,p7,p8,p9>
		(index,param1,param2,param3,param4,param5,param6,param7,param8,param9);
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10>
uint ShaderUniformManager::addDataToUniformBlock10(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8,const p9& param9,const p10& param10)
{
	return uniformBlocks.addDataToUniformBlock10<p1,p2,p3,p4,p5,p6,p7,p8,p9,p10>
		(index,param1,param2,param3,param4,param5,param6,param7,param8,param9,param10);
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10, class p11>
uint ShaderUniformManager::addDataToUniformBlock11(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8,const p9& param9,const p10& param10,const p11& param11)
{
	return uniformBlocks.addDataToUniformBlock11<p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11>
		(index,param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11);
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10, class p11, class p12>
uint ShaderUniformManager::addDataToUniformBlock12(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8,const p9& param9,const p10& param10,const p11& param11,const p12& param12)
{
	return uniformBlocks.addDataToUniformBlock12<p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12>
		(index,param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11,param12);
}

#pragma endregion

#endif