#ifndef UNIFORM_BLOCK_MANAGER_H
#define UNIFORM_BLOCK_MANAGER_H
#include "MyTypeDefs.h"
#include <vector>
#include "UniformBlockInfo.h"

class UniformBlockManager
{
	static uint nextBindingID;
	uint programID;

	std::vector<UniformBlockInfo*> uniformBlocks;

	static UniformBlockManager instance;
public:
	UniformBlockManager(uint programID);
	~UniformBlockManager(void);

	uint addUniformBlock(const char* uniformBlockName, const char* uniformStructName);
	void activateBlocks()const;

	uint getUniformBlockID(const char* uniformBlockName)const;

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
uint UniformBlockManager::addDataToUniformBlock1(uint index,const p1& param1)
{
	return uniformBlocks[index]->addData1<p1>(param1);
}

template <class p1,class p2>
uint UniformBlockManager::addDataToUniformBlock2(uint index, const p1& param1,const p2& param2)
{
	return uniformBlocks[index]->addData2<p1,p2>(param1,param2);
}

template <class p1,class p2, class p3>
uint UniformBlockManager::addDataToUniformBlock3(uint index, const p1& param1,const p2& param2,const p3& param3)
{
	return uniformBlocks[index]->addData3<p1,p2,p3>(param1,param2,param3);
}

template <class p1,class p2, class p3, class p4>
uint UniformBlockManager::addDataToUniformBlock4(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4)
{
	return uniformBlocks[index]->addData4<p1,p2,p3,p4>(param1,param2,param3,param4);
}

template <class p1,class p2, class p3, class p4, class p5>
uint UniformBlockManager::addDataToUniformBlock5(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5)
{
	return uniformBlocks[index]->addData5<p1,p2,p3,p4,p5>
		(param1,param2,param3,param4,param5);
}

template <class p1,class p2, class p3, class p4, class p5, class p6>
uint UniformBlockManager::addDataToUniformBlock6(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6)
{
	return uniformBlocks[index]->addData6<p1,p2,p3,p4,p5,p6>
		(param1,param2,param3,param4,param5,param6);
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7>
uint UniformBlockManager::addDataToUniformBlock7(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7)
{
	return uniformBlocks[index]->addData7<p1,p2,p3,p4,p5,p6,p7>
		(param1,param2,param3,param4,param5,param6,param7);
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8>
uint UniformBlockManager::addDataToUniformBlock8(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8)
{
	return uniformBlocks[index]->addData8<p1,p2,p3,p4,p5,p6,p7,p8>
		(param1,param2,param3,param4,param5,param6,param7,param8);
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9>
uint UniformBlockManager::addDataToUniformBlock9(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8,const p9& param9)
{
	return uniformBlocks[index]->addData9<p1,p2,p3,p4,p5,p6,p7,p8,p9>
		(param1,param2,param3,param4,param5,param6,param7,param8,param9);
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10>
uint UniformBlockManager::addDataToUniformBlock10(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8,const p9& param9,const p10& param10)
{
	return uniformBlocks[index]->addData10<p1,p2,p3,p4,p5,p6,p7,p8,p9,p10>
		(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10);
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10, class p11>
uint UniformBlockManager::addDataToUniformBlock11(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8,const p9& param9,const p10& param10,const p11& param11)
{
	return uniformBlocks[index]->addData11<p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11>
		(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11);
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10, class p11, class p12>
uint UniformBlockManager::addDataToUniformBlock12(uint index, const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8,const p9& param9,const p10& param10,const p11& param11,const p12& param12)
{
	return uniformBlocks[index]->addData12<p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12>
		(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11,param12);
}
#pragma endregion
#endif