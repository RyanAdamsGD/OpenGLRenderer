#ifndef GEOMETRY_INFO_H
#define GEOMETRY_INFO_H
#include "MyTypeDefs.h"
#include "FastDelegate.h"
#include <vector>

class ShapeData;

class GeometryInfo
{
	struct Buffer
	{
		uint bufferID;
		uint currentOffset;
		Buffer():bufferID(0),currentOffset(0){}
	};
	static std::vector<Buffer> vertexBuffers;
	static std::vector<Buffer> indexBuffers;
	
	uint numIndices;
	uint numVerts;
	ushort sizeOfIndex;
	//mutable so we can steal the values in copy operations
	mutable uint vertexBufferID;
	mutable uint indexBufferID;
	uint vertexBufferOffset;
	uint indexBufferOffset;
	ushort stride;
	ushort vertexPositionOffset;
	ushort vertexColorOffset;
	ushort vertexNormalOffset;
	ushort vertexUVOffset;
	ushort vertexTangentOffset;
	ushort vertexBitangetOffset;
	DrawType drawType;

	bool setUpBuffers(const void* vertexData, const void* indexData, uint vertexBufferSize, uint indexBufferSize);
	void genNewBuffer(Buffer* buffer,uint& currentBufferCount, uint bufferID);

	void getVertexBufferData(void*& data)const;
	void getIndexBufferData(ushort*& data)const;

	static std::vector<fastdelegate::FastDelegate6<uint, uint, uint, uint, uint, uint>> deletionEvent;

public:
	GeometryInfo();

	GeometryInfo(const void* vertexData,const void* indexData, uint vertexBufferSize,
		uint indexBufferSize, uint numVerts,
		uint numIndices,uint vertexPositionOffset = 0,
		uint vertexUVOffset = 0,uint vertexNormalOffset = 0,
		uint vertexColorOffset = 0, DrawType drawType = drawTriangle);
	
	GeometryInfo(const ShapeData& shapeData, DrawType drawType = drawTriangle);

	//The array passed in will be overwritten and count will be set
	static void loadFile(const char* fileName, DrawType drawType, GeometryInfo* array, uint& count);
	static void loadFile(const char* fileName, GeometryInfo* array, uint& count);

	GeometryInfo(const GeometryInfo& rhs);

	GeometryInfo& operator =(const GeometryInfo& rhs);

	~GeometryInfo(void);

	///vertexBufferID vertexStartOffset vertexAmountRemoved indexBufferID indexStartOffset indexAmountRemoved
	static void addToDeletionEvent(fastdelegate::FastDelegate6<uint, uint, uint, uint, uint, uint> eventListener);

	uint getVertexBufferID() const { return vertexBufferID; }
	uint getIndexBufferID() const { return indexBufferID; }
	uint getVertexBufferOffset() const { return vertexBufferOffset; }
	uint getIndexBufferOffset() const { return indexBufferOffset; }
	uint getNumIndicies() const { return numIndices; }
	DrawType getDrawType() const { return drawType; }

	void writeToFile(const char* fileName)const;
};

#endif