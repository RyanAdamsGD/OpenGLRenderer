#include "GeometryInfo.h"
#include <ShapeData.h>
#include <Vertex.h>
#include <GL\glew.h>
#include "Logger.h"
#include <assert.h>
#include "ShapeLoader.h"
#include "ShapeExporter.h"

std::vector<GeometryInfo::Buffer> GeometryInfo::vertexBuffers; 
std::vector<GeometryInfo::Buffer> GeometryInfo::indexBuffers; 
std::vector<fastdelegate::FastDelegate6<uint, uint, uint, uint, uint, uint>> GeometryInfo::deletionEvent;

GeometryInfo::GeometryInfo()
	:numIndices(0),numVerts(0),sizeOfIndex(0),vertexBufferID(0),indexBufferID(0),vertexBufferOffset(0),
	indexBufferOffset(0),vertexPositionOffset(0),vertexColorOffset(0),
	vertexNormalOffset(0),vertexUVOffset(0),stride(0),
	drawType(drawTriangle)
{
}


GeometryInfo::GeometryInfo(const void* vertexData,const void* indexData, uint vertexBufferSize,
		uint indexBufferSize, uint numVerts,
		uint numIndices,uint vertexPositionOffset,
		uint vertexUVOffset,uint vertexNormalOffset,
		uint vertexColorOffset, DrawType drawType)
		:numIndices(numIndices),numVerts(numVerts),vertexBufferID(0),indexBufferID(0),vertexBufferOffset(0),
		indexBufferOffset(0),vertexPositionOffset(vertexPositionOffset),vertexColorOffset(vertexColorOffset),
		vertexNormalOffset(vertexNormalOffset),vertexUVOffset(vertexUVOffset),drawType(drawType)
{
	stride = vertexBufferSize / numVerts;
#ifdef USE_LOGGER
	if(vertexBufferSize % stride != 0)
		LOGS("Stride does not evenly divide into size of vertex Buffer. \n\
			Please check the stride and buffer size.");
#endif
	assert(vertexBufferSize % stride == 0);
	sizeOfIndex = indexBufferSize / numIndices;
#ifdef USE_LOGGER
	if(indexBufferSize % sizeOfIndex != 0)
		LOGS("Size of the index does not evenly divide into size of index buffer. \n\
			Please check the size of index and buffer size.");
#endif
	assert(indexBufferSize % sizeOfIndex == 0);
	setUpBuffers(vertexData,indexData,vertexBufferSize,indexBufferSize);
}

GeometryInfo::GeometryInfo(const ShapeData& shapeData, DrawType drawType)
	:numIndices(shapeData.numIndices),numVerts(shapeData.numVerts),sizeOfIndex(sizeof(ushort)),vertexBufferID(0),indexBufferID(0),vertexBufferOffset(0),
	indexBufferOffset(0),vertexPositionOffset(Vertex::POSITION_OFFSET),vertexColorOffset(Vertex::COLOR_OFFSET),
	vertexNormalOffset(Vertex::NORMAL_OFFSET),vertexUVOffset(Vertex::UV_OFFSET),stride(Vertex::STRIDE),
	drawType(drawType)
{
	setUpBuffers(shapeData.verts,shapeData.indices,shapeData.vertexBufferSize(),shapeData.indexBufferSize());
}

GeometryInfo::GeometryInfo(const GeometryInfo& rhs)
	:numIndices(rhs.numIndices),numVerts(rhs.numVerts),vertexBufferID(rhs.vertexBufferID),indexBufferID(rhs.indexBufferID),
	sizeOfIndex(rhs.sizeOfIndex),vertexBufferOffset(rhs.vertexBufferOffset),indexBufferOffset(rhs.indexBufferOffset),stride(rhs.stride),
	vertexPositionOffset(rhs.vertexPositionOffset),vertexColorOffset(rhs.vertexColorOffset),vertexTangentOffset(rhs.vertexTangentOffset),
	vertexBitangetOffset(rhs.vertexBitangetOffset),drawType(rhs.drawType)
{
	//optimization
	//rather than deleting the data and re-adding it to one of the buffers i.e. deepcopy
	//we steal the IDs from the object being copied
	rhs.vertexBufferID = 0;
	rhs.indexBufferID = 0;
}

GeometryInfo& GeometryInfo::operator =(const GeometryInfo& rhs)
{
	numIndices = rhs.numIndices;
	numVerts = rhs.numVerts;
	sizeOfIndex = rhs.sizeOfIndex;
	vertexBufferID = rhs.vertexBufferID;
	indexBufferID = rhs.indexBufferID;
	vertexBufferOffset = rhs.vertexBufferOffset;
	indexBufferOffset = rhs.indexBufferOffset;
	stride = rhs.stride;
	vertexPositionOffset = rhs.vertexPositionOffset;
	vertexColorOffset = rhs.vertexColorOffset;
	vertexTangentOffset = rhs.vertexTangentOffset;
	vertexBitangetOffset = rhs.vertexBitangetOffset;
	drawType = rhs.drawType;

	//optimization
	//rather than deleting the data and re-adding it to one of the buffers i.e. deepcopy
	//we steal the IDs from the object being copied
	rhs.vertexBufferID = 0;
	rhs.indexBufferID = 0;

	return *this;
}

GeometryInfo::~GeometryInfo(void)
{
	uint size;
	Buffer* buffer = NULL;
	if(vertexBufferID != 0)
	{
		size = vertexBuffers.size();
		buffer = NULL;
		for(uint i=0;i<size && buffer == NULL;i++)
		{
			if(vertexBuffers[i].bufferID == vertexBufferID)
				buffer = &vertexBuffers[i];
		}
		//we better find a static buffer container storing the buffer's current total offset
		//also assert since we don't know the order for destructor calls, the logger may not exist
		assert(buffer != NULL);
	}

	if(indexBufferID != 0)
	{
		size = indexBuffers.size();
		buffer = NULL;
		for(uint i=0;i<size && buffer == NULL;i++)
		{
			if(indexBuffers[i].bufferID == indexBufferID)
				buffer = &indexBuffers[i];
		}
		//we better find a static buffer container storing the buffer's current total offset
		//also assert since we don't know the order for destructor calls, the logger may not exist
		assert(buffer != NULL);
	}
	if(indexBufferID != 0 && vertexBufferID != 0)
	{
		uint size = deletionEvent.size();
		for(uint i=0;i<size;i++)
		{
			deletionEvent[i](vertexBufferID,vertexBufferOffset,stride * numVerts,indexBufferID,indexBufferOffset,sizeOfIndex * numIndices);
		}
	}
	//if we created a vertex buffer without a index buffer or viceversa something is very wrong
	//also assert since we don't know the order for destructor calls, the logger may not exist
	assert((indexBufferID == 0 && vertexBufferID == 0) || (indexBufferID != 0 && vertexBufferID != 0));
}

void GeometryInfo::addToDeletionEvent(fastdelegate::FastDelegate6<uint, uint, uint, uint, uint, uint> eventListener)
{
	//TODO: figure out what this was for and why its here
	deletionEvent.push_back(eventListener);
}

bool GeometryInfo::setUpBuffers(const void* vertexData, const void* indexData, uint vertexBufferSize, uint indexBufferSize)
{
	bool vertexAdded = false;
	bool indexAdded = false;
	//assume less than a meg of data is coming down
	assert(vertexBufferSize<MEGABYTE);
	assert(indexBufferSize<MEGABYTE);
	uint vertexBufferCount = vertexBuffers.size();
	uint indexBufferCount = indexBuffers.size();

	//loop through all the current buffers to look for open spaces to place the data
	for(uint i=0;i<vertexBufferCount && !vertexAdded;i++)
	{
		//check if there is room at the end of the buffer
		if(vertexBuffers[i].currentOffset + vertexBufferSize < MEGABYTE)
		{
			vertexBufferID = vertexBuffers[i].bufferID;
			vertexBufferOffset = vertexBuffers[i].currentOffset;

			glBindBuffer(GL_ARRAY_BUFFER,vertexBufferID);
			glBufferSubData(GL_ARRAY_BUFFER,vertexBufferOffset,vertexBufferSize,vertexData);
			vertexBuffers[i].currentOffset += vertexBufferSize;
			vertexAdded=true;
		}
	}

	for(uint i=0;i<indexBufferCount && !indexAdded;i++)
	{
		//check if there is room at the end of the buffer
			if(indexBuffers[i].currentOffset + indexBufferSize < MEGABYTE)
			{
				indexBufferID = indexBuffers[i].bufferID;
				indexBufferOffset = indexBuffers[i].currentOffset;

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexBufferID);
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,indexBufferOffset,indexBufferSize,indexData);
				indexBuffers[i].currentOffset += indexBufferSize;
				indexAdded=true;
			}
	}

	//if we werent able to find a buffer that had enough room for the
	//vertex data we create a new one
	if(!vertexAdded)
	{
		glGenBuffers(1,&vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER,vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER,MEGABYTE,NULL,GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER,0,vertexBufferSize,vertexData);
		vertexBuffers.push_back(Buffer());
		vertexBuffers[vertexBufferCount].currentOffset =vertexBufferSize;
		vertexBuffers[vertexBufferCount].bufferID= vertexBufferID;
		vertexBufferOffset = 0;
		vertexAdded=true;
	}
	//if we werent able to find a buffer that had enough room for the
	//index data we create a new one
	if(!indexAdded)
	{
		glGenBuffers(1,&indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,MEGABYTE,NULL,GL_STATIC_DRAW);		
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,indexBufferSize,indexData);
		indexBuffers.push_back(Buffer());
		indexBuffers[indexBufferCount].currentOffset = indexBufferSize;
		indexBuffers[indexBufferCount].bufferID = indexBufferID;
		indexBufferOffset = 0;
		indexAdded=true;
	}
	return indexAdded && vertexAdded;
}

void GeometryInfo::loadFile(const char* fileName, DrawType drawType, GeometryInfo* geometryInfos, uint& count)
{
	ShapeData* shapeData;
	bool successfullLoad = ShapeLoader::loadFile(fileName, shapeData, count);

	if(!successfullLoad)
	{
		geometryInfos = NULL;
		count = 0;

#ifdef USE_LOGGER
		char buffer[70];
		sprintf_s(buffer,50,fileName);
		short size = strlen(fileName);
		size = size > 50 ? 50 : size;
		sprintf(buffer + size," failed to load.");
		LOGW(buffer);
#endif
		return;
	}

	geometryInfos = new GeometryInfo[count];
	for(uint i = 0; i < count; i++)
	{
		geometryInfos[i] = GeometryInfo(shapeData[i],drawType);
	}
}

void GeometryInfo::loadFile(const char* fileName, GeometryInfo* geometryInfos, uint& count)
{
	ShapeData* shapeData;
	bool successfullLoad = ShapeLoader::loadFile(fileName, shapeData, count);

	if(!successfullLoad)
	{
		geometryInfos = NULL;
		count = 0;

#ifdef USE_LOGGER
		char buffer[70];
		sprintf_s(buffer,50,fileName);
		short size = strlen(fileName);
		size = size > 50 ? 50 : size;
		sprintf(buffer + size," failed to load.");
		LOGW(buffer);
#endif
		return;
	}

	geometryInfos = new GeometryInfo[count];
	for(uint i = 0; i < count; i++)
	{
		geometryInfos[i] = GeometryInfo(shapeData[i],drawTriangle);
	}
}

void GeometryInfo::writeToFile(const char* fileName)const
{
	ShapeData data;

	void* verts = NULL;
	data.numVerts = numVerts;
	ushort* indices = NULL;
	data.numIndices = numIndices;

	getVertexBufferData(verts);
	getIndexBufferData(indices);
	data.verts = reinterpret_cast<Vertex*>(verts);
	data.indices = indices;


	//TODO:: add these
	data.numAnimations = 0;
	data.numBones = 0;
	data.normalMapFileName = NULL;
	data.frameCount = 0;
	data.skeleton = 0;
	data.textureFileName = NULL;
	data.animation = NULL;

	Shape_Exporter.writeShapeData(&data,1, fileName);
}

void GeometryInfo::getVertexBufferData(void*& data)const
{
	data = new Vertex[numVerts];
	glBindBuffer(GL_ARRAY_BUFFER,vertexBufferID);
	glGetBufferSubData(GL_ARRAY_BUFFER,vertexBufferOffset,numVerts * stride,data);
}

void GeometryInfo::getIndexBufferData(ushort*& data)const
{
	data = new ushort[numIndices];
	glBindBuffer(GL_ARRAY_BUFFER,indexBufferID);
	glGetBufferSubData(GL_ARRAY_BUFFER,indexBufferOffset,numIndices * sizeof(ushort),data);
}