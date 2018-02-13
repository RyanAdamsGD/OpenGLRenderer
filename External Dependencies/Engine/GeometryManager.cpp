#include "GeometryManager.h"
#include "ShapeData.h"
#include "Vertex.h"
#include <GL\glew.h>
#include "Logger.h"
#include "GeometryInfo.h"

GeometryManager GeometryManager::instance;

GeometryManager::GeometryManager(void)
	:defaultShapeDataLayouts(),totalGeometryCount(0)
{
	defaultShapeDataLayouts.addLayout(3,FLOAT3,Vertex::STRIDE,Vertex::POSITION_OFFSET);
	defaultShapeDataLayouts.addLayout(4,FLOAT4,Vertex::STRIDE,Vertex::COLOR_OFFSET);
	defaultShapeDataLayouts.addLayout(3,FLOAT3,Vertex::STRIDE,Vertex::NORMAL_OFFSET);
	defaultShapeDataLayouts.addLayout(2,FLOAT2,Vertex::STRIDE,Vertex::UV_OFFSET);
	defaultShapeDataLayouts.addLayout(3,FLOAT3,Vertex::STRIDE,Vertex::TANGENT_OFFSET);
	defaultShapeDataLayouts.addLayout(3,FLOAT3,Vertex::STRIDE,Vertex::BITANGENT_OFFSET);
}

GeometryManager::~GeometryManager(void)
{
	uint size = VAOs.size();
	uint error = glGetError();
	if(size > 0)
	{
		//TODO: make this not explode
		//glDeleteVertexArrays(size,&VAOs[0]);
		VAOs.clear();
	}
	for(uint i=0;i<totalGeometryCount;i++)
	{
		if(geometryInfos[i] != NULL)
		{
			delete geometryInfos[i];
			geometryInfos[i] = NULL;
		}
	}
	geometryInfos.clear();
	totalGeometryCount = geometryInfos.size();
}

GeometryInfo* GeometryManager::getGeometry(uint index)
{
	if(index > totalGeometryCount)
	{
#ifdef USE_LOGGER
		char message[70];
		sprintf(message,"Requested geometry index: %u is out of range.", index);
		LOGW(message);
#endif
		return NULL;
	}

	return geometryInfos[index];
}

uint GeometryManager::getVAO(uint index)
{
	if(index > totalGeometryCount)
	{
#ifdef USE_LOGGER
		char message[70];
		sprintf(message,"Requested VAO index: %u is out of range.", index);
		LOGW(message);
#endif
		return NULL;
	}

	return VAOs[index];
}

GeometryManager::LayoutGeometrySet::~LayoutGeometrySet()
{
	//check so that we dont ever try to delete the default one that isn't on the heap
	if(layoutManager != NULL && layoutManager != &Geometry_Manager.defaultShapeDataLayouts)
	{
		delete layoutManager;
		layoutManager = NULL;
	}
}

uint GeometryManager::addGeometry(const void* vertexData,const void* indexData, uint vertexBufferSize,
	uint indexBufferSize, uint numVerts,
	uint numIndicies,uint vertexPositionOffset,
	uint vertexUVOffset,uint vertexNormalOffset,
	uint vertexColorOffset, DrawType drawType)
{
	geometryInfos.push_back(new GeometryInfo(vertexData,indexData,vertexBufferSize,indexBufferSize,numVerts,numIndicies,
		vertexPositionOffset,vertexUVOffset,vertexNormalOffset,vertexColorOffset,drawType));
	uint index = geometryInfos.size()-1;
	GeometryInfo* geometryInfo = geometryInfos[index];

	//set up the vertex array object
	uint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER,geometryInfo->getVertexBufferID());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,geometryInfo->getIndexBufferID());
	defaultShapeDataLayouts.activateLayouts(geometryInfo->getVertexBufferOffset());
	glBindVertexArray(0);
	VAOs.push_back(vao);

	//add the layoutManager to our sets and return the index
	layoutGeometrySets.push_back(LayoutGeometrySet(index,&defaultShapeDataLayouts));
	totalGeometryCount = geometryInfos.size();
	return index;
}

uint GeometryManager::addGeometry(const ShapeData& shapeData, DrawType drawType)
{
	geometryInfos.push_back(new GeometryInfo(shapeData,drawType));
	uint index = geometryInfos.size()-1;
	GeometryInfo* geometryInfo = geometryInfos[index];

	//set up the vertex array object
	uint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER,geometryInfo->getVertexBufferID());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,geometryInfo->getIndexBufferID());
	defaultShapeDataLayouts.activateLayouts(geometryInfo->getVertexBufferOffset());
	glBindVertexArray(0);
	VAOs.push_back(vao);

	//add the layoutManager to our sets and return the index
	layoutGeometrySets.push_back(LayoutGeometrySet(index,&defaultShapeDataLayouts));
	totalGeometryCount = geometryInfos.size();
	return index;
}

uint GeometryManager::addGeometry(const ShaderLayoutManager& layoutManager,const void* vertexData,const void* indexData, uint vertexBufferSize,
	uint indexBufferSize, uint numVerts,
	uint numIndicies,uint vertexPositionOffset,
	uint vertexUVOffset,uint vertexNormalOffset,
	uint vertexColorOffset, DrawType drawType)
{
	geometryInfos.push_back(new GeometryInfo(vertexData,indexData,vertexBufferSize,indexBufferSize,numVerts,numIndicies,
		vertexPositionOffset,vertexUVOffset,vertexNormalOffset,vertexColorOffset,drawType));
	uint index = geometryInfos.size() - 1;
	GeometryInfo* geometryInfo = geometryInfos[index];

	//set up the vertex array object
	uint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER,geometryInfo->getVertexBufferID());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,geometryInfo->getIndexBufferID());
	layoutManager.activateLayouts(geometryInfo->getVertexBufferOffset());
	glBindVertexArray(0);
	VAOs.push_back(vao);

	//add the layoutManager to our sets and return the index
	ShaderLayoutManager *storedLayoutManager = new ShaderLayoutManager(layoutManager);
	layoutGeometrySets.push_back(LayoutGeometrySet(index,storedLayoutManager));
	totalGeometryCount = geometryInfos.size();
	return index;
}

uint GeometryManager::addGeometry(const ShaderLayoutManager& layoutManager,const ShapeData& shapeData, DrawType drawType)
{
	geometryInfos.push_back(new GeometryInfo(shapeData,drawType));
	uint index = geometryInfos.size() - 1;
	GeometryInfo* geometryInfo = geometryInfos[index];

	//set up the vertex array object
	uint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER,geometryInfo->getVertexBufferID());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,geometryInfo->getIndexBufferID());
	layoutManager.activateLayouts(geometryInfo->getVertexBufferOffset());
	glBindVertexArray(0);
	VAOs.push_back(vao);

	//add the layoutManager to our sets and return the index
	ShaderLayoutManager *storedLayoutManager = new ShaderLayoutManager(layoutManager);
	layoutGeometrySets.push_back(LayoutGeometrySet(index,storedLayoutManager));
	totalGeometryCount = geometryInfos.size();
	return index;
}

void GeometryManager::removeGeometryInfo(uint index)
{
	//TODO:: make sure the vao get removed properly
	//it may already work in destructor once it is fixed
	//but cleaning it up here would be more appropriate

	if(index > totalGeometryCount)
	{
#ifdef USE_LOGGER
		char message[70];
		sprintf(message,"Geometry index to delete: %u is out of range.", index);
		LOGW(message);
#endif
		return;
	}
	delete geometryInfos[index];
	geometryInfos[index] = NULL;

}