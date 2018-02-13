#ifndef UNIFORM_BLOCK_INFO_H
#define UNIFORM_BLOCK_INFO_H
#include "MyTypeDefs.h"
#include <memory>
#include <vector>

#define MAX_UNIFORM_PARAMETERS 12

class UniformBlockInfo
{
	uint bindPoint;
	uint bufferID;
	uint programID;
	uint location;
	const char* name;
	int blockSize;
	uint blockCount;
	uint totalBlockCount;

	int numParams;
	int* offsets;
	uchar* data;
	//indicies returned to users are into this array
	//which hold pointers into the actual array of data
	uchar** dataPtrs;


	void bindBlock();
	//finds the next open index
	//and returns a pointer to the start of the new block
	uchar* addNewDataBlock(uint& index);
		
public:
	~UniformBlockInfo(void);
	void activateUniformBlock()const;
	UniformBlockInfo(uint programID, const char* uniformBlockName, const char* uniformStructName, uint blockID);
	
	
#pragma region Template Function Declarations
	template <class p1>
	uint addData1(const p1& param1);

	template <class p1,class p2>
	uint addData2(const p1& param1,const p2& param2);

	template <class p1,class p2,class p3>
	uint addData3(const p1& param1,const p2& param2,const p3& param3);

	template <class p1,class p2, class p3, class p4>
	uint addData4(const p1& param1,const p2& param2,const p3& param3,const p4& param4);

	template <class p1,class p2, class p3, class p4, class p5>
	uint addData5(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5);

	template <class p1,class p2, class p3, class p4, class p5, class p6>
	uint addData6(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6);

	template <class p1,class p2, class p3, class p4, class p5, class p6, class p7>
	uint addData7(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7);

	template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8>
	uint addData8(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
		const p8& param8);

	template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9>
	uint addData9(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
		const p8& param8,const p9& param9);

	template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10>
	uint addData10(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
		const p8& param8,const p9& param9,const p10& param10);

	template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10, class p11>
	uint addData11(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
		const p8& param8,const p9& param9,const p10& param10,const p11& param11);

	template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10, class p11, class p12>
	uint addData12(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
		const p8& param8,const p9& param9,const p10& param10,const p11& param11,const p12& param12);

#pragma endregion

	void removeData(uint index);

	inline const char* getUniformBlockName() { return name; }
};

#pragma region Template Function Definitions

template <class p1>
uint UniformBlockInfo::addData1(const p1& param1)
{
	uint index;
	uchar* newDataBlock = addNewDataBlock(index);

	memcpy(&newDataBlock[offsets[0]],&param1,sizeof(p1));
	
	bindBlock();
	return index;
}

template <class p1,class p2>
uint UniformBlockInfo::addData2(const p1& param1,const p2& param2)
{
	uint index;
	uchar* newDataBlock = addNewDataBlock(index);

	memcpy(&newDataBlock[offsets[0]],&param1,sizeof(p1));
	memcpy(&newDataBlock[offsets[1]],&param2,sizeof(p2));
	
	bindBlock();
	return index;
}

template <class p1,class p2,class p3>
uint UniformBlockInfo::addData3(const p1& param1,const p2& param2,const p3& param3)
{
	uint index;
	uchar* newDataBlock = addNewDataBlock(index);

	memcpy(&newDataBlock[offsets[0]],&param1,sizeof(p1));
	memcpy(&newDataBlock[offsets[1]],&param2,sizeof(p2));
	memcpy(&newDataBlock[offsets[2]],&param3,sizeof(p3));
	
	bindBlock();
	return index;
}

template <class p1,class p2, class p3, class p4>
uint UniformBlockInfo::addData4(const p1& param1,const p2& param2,const p3& param3,const p4& param4)
{
	uint index;
	uchar* newDataBlock = addNewDataBlock(index);

	memcpy(&newDataBlock[offsets[0]],&param1,sizeof(p1));
	memcpy(&newDataBlock[offsets[1]],&param2,sizeof(p2));
	memcpy(&newDataBlock[offsets[2]],&param3,sizeof(p3));
	memcpy(&newDataBlock[offsets[3]],&param4,sizeof(p4));
	
	bindBlock();
	return index;
}

template <class p1,class p2, class p3, class p4, class p5>
uint UniformBlockInfo::addData5(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5)
{
	uint index;
	uchar* newDataBlock = addNewDataBlock(index);

	memcpy(&newDataBlock[offsets[0]],&param1,sizeof(p1));
	memcpy(&newDataBlock[offsets[1]],&param2,sizeof(p2));
	memcpy(&newDataBlock[offsets[2]],&param3,sizeof(p3));
	memcpy(&newDataBlock[offsets[3]],&param4,sizeof(p4));
	memcpy(&newDataBlock[offsets[4]],&param5,sizeof(p5));
	
	return index;
}

template <class p1,class p2, class p3, class p4, class p5, class p6>
uint UniformBlockInfo::addData6(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6)
{
	uint index;
	uchar* newDataBlock = addNewDataBlock(index);

	memcpy(&newDataBlock[offsets[0]],&param1,sizeof(p1));
	memcpy(&newDataBlock[offsets[1]],&param2,sizeof(p2));
	memcpy(&newDataBlock[offsets[2]],&param3,sizeof(p3));
	memcpy(&newDataBlock[offsets[3]],&param4,sizeof(p4));
	memcpy(&newDataBlock[offsets[4]],&param5,sizeof(p5));
	memcpy(&newDataBlock[offsets[5]],&param6,sizeof(p6));
	
	bindBlock();
	return index;
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7>
uint UniformBlockInfo::addData7(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7)
{
	uint index;
	uchar* newDataBlock = addNewDataBlock(index);

	memcpy(&newDataBlock[offsets[0]],&param1,sizeof(p1));
	memcpy(&newDataBlock[offsets[1]],&param2,sizeof(p2));
	memcpy(&newDataBlock[offsets[2]],&param3,sizeof(p3));
	memcpy(&newDataBlock[offsets[3]],&param4,sizeof(p4));
	memcpy(&newDataBlock[offsets[4]],&param5,sizeof(p5));
	memcpy(&newDataBlock[offsets[5]],&param6,sizeof(p6));
	memcpy(&newDataBlock[offsets[6]],&param7,sizeof(p7));
	
	return index;
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8>
uint UniformBlockInfo::addData8(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8)
{
	uint index;
	uchar* newDataBlock = addNewDataBlock(index);

	memcpy(&newDataBlock[offsets[0]],&param1,sizeof(p1));
	memcpy(&newDataBlock[offsets[1]],&param2,sizeof(p2));
	memcpy(&newDataBlock[offsets[2]],&param3,sizeof(p3));
	memcpy(&newDataBlock[offsets[3]],&param4,sizeof(p4));
	memcpy(&newDataBlock[offsets[4]],&param5,sizeof(p5));
	memcpy(&newDataBlock[offsets[5]],&param6,sizeof(p6));
	memcpy(&newDataBlock[offsets[6]],&param7,sizeof(p7));
	memcpy(&newDataBlock[offsets[7]],&param8,sizeof(p8));
	
	bindBlock();
	return index;
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9>
uint UniformBlockInfo::addData9(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8,const p9& param9)
{
	uint index;
	uchar* newDataBlock = addNewDataBlock(index);

	memcpy(&newDataBlock[offsets[0]],&param1,sizeof(p1));
	memcpy(&newDataBlock[offsets[1]],&param2,sizeof(p2));
	memcpy(&newDataBlock[offsets[2]],&param3,sizeof(p3));
	memcpy(&newDataBlock[offsets[3]],&param4,sizeof(p4));
	memcpy(&newDataBlock[offsets[4]],&param5,sizeof(p5));
	memcpy(&newDataBlock[offsets[5]],&param6,sizeof(p6));
	memcpy(&newDataBlock[offsets[6]],&param7,sizeof(p7));
	memcpy(&newDataBlock[offsets[7]],&param8,sizeof(p8));
	memcpy(&newDataBlock[offsets[8]],&param9,sizeof(p9));
	
	bindBlock();
	return index;
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10>
uint UniformBlockInfo::addData10(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8,const p9& param9,const p10& param10)
{
	uint index;
	uchar* newDataBlock = addNewDataBlock(index);

	memcpy(&newDataBlock[offsets[0]],&param1,sizeof(p1));
	memcpy(&newDataBlock[offsets[1]],&param2,sizeof(p2));
	memcpy(&newDataBlock[offsets[2]],&param3,sizeof(p3));
	memcpy(&newDataBlock[offsets[3]],&param4,sizeof(p4));
	memcpy(&newDataBlock[offsets[4]],&param5,sizeof(p5));
	memcpy(&newDataBlock[offsets[5]],&param6,sizeof(p6));
	memcpy(&newDataBlock[offsets[6]],&param7,sizeof(p7));
	memcpy(&newDataBlock[offsets[7]],&param8,sizeof(p8));
	memcpy(&newDataBlock[offsets[8]],&param9,sizeof(p9));
	memcpy(&newDataBlock[offsets[9]],&param10,sizeof(p10));
	
	bindBlock();
	return index;
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10, class p11>
uint UniformBlockInfo::addData11(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8,const p9& param9,const p10& param10,const p11& param11)
{
	uint index;
	uchar* newDataBlock = addNewDataBlock(index);

	memcpy(&newDataBlock[offsets[0]],&param1,sizeof(p1));
	memcpy(&newDataBlock[offsets[1]],&param2,sizeof(p2));
	memcpy(&newDataBlock[offsets[2]],&param3,sizeof(p3));
	memcpy(&newDataBlock[offsets[3]],&param4,sizeof(p4));
	memcpy(&newDataBlock[offsets[4]],&param5,sizeof(p5));
	memcpy(&newDataBlock[offsets[5]],&param6,sizeof(p6));
	memcpy(&newDataBlock[offsets[6]],&param7,sizeof(p7));
	memcpy(&newDataBlock[offsets[7]],&param8,sizeof(p8));
	memcpy(&newDataBlock[offsets[8]],&param9,sizeof(p9));
	memcpy(&newDataBlock[offsets[9]],&param10,sizeof(p10));
	memcpy(&newDataBlock[offsets[10]],&param11,sizeof(p11));
	
	bindBlock();
	return index;
}

template <class p1,class p2, class p3, class p4, class p5, class p6, class p7, class p8, class p9, class p10, class p11, class p12>
uint UniformBlockInfo::addData12(const p1& param1,const p2& param2,const p3& param3,const p4& param4,const p5& param5,const p6& param6,const p7& param7,
	const p8& param8,const p9& param9,const p10& param10,const p11& param11,const p12& param12)
{
	uint index;
	uchar* newDataBlock = addNewDataBlock(index);

	memcpy(&newDataBlock[offsets[0]],&param1,sizeof(p1));
	memcpy(&newDataBlock[offsets[1]],&param2,sizeof(p2));
	memcpy(&newDataBlock[offsets[2]],&param3,sizeof(p3));
	memcpy(&newDataBlock[offsets[3]],&param4,sizeof(p4));
	memcpy(&newDataBlock[offsets[4]],&param5,sizeof(p5));
	memcpy(&newDataBlock[offsets[5]],&param6,sizeof(p6));
	memcpy(&newDataBlock[offsets[6]],&param7,sizeof(p7));
	memcpy(&newDataBlock[offsets[7]],&param8,sizeof(p8));
	memcpy(&newDataBlock[offsets[8]],&param9,sizeof(p9));
	memcpy(&newDataBlock[offsets[9]],&param10,sizeof(p10));
	memcpy(&newDataBlock[offsets[10]],&param11,sizeof(p11));
	memcpy(&newDataBlock[offsets[11]],&param12,sizeof(p12));
	
	bindBlock();
	return index;
}

#pragma endregion

#endif