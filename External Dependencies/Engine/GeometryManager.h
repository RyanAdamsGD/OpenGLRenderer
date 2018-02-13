#ifndef GEOMETRY_MANAGER_H
#define GEOMETRY_MANAGER_H
#include "MyTypeDefs.h"
#include "ShaderLayoutManager.h"
#include <vector>
#include "ExportHeader.h"
class GeometryInfo;
class ShapeData;

class ENGINE_SHARED GeometryManager
{
	std::vector<GeometryInfo*> geometryInfos;
	uint totalGeometryCount;

	struct LayoutGeometrySet
	{
		uint geometryIndex;
		ShaderLayoutManager* layoutManager;
		LayoutGeometrySet(uint geometryIndex,ShaderLayoutManager* layoutManager)
			:geometryIndex(geometryIndex),layoutManager(layoutManager) {}
		~LayoutGeometrySet();
	};
	//friend the inner class so we can make a delete check in the destructor
	friend class LayoutGeometrySet;
	ShaderLayoutManager defaultShapeDataLayouts;

	std::vector<LayoutGeometrySet> layoutGeometrySets;
	static GeometryManager instance;
	GeometryManager(void);
	GeometryManager(const GeometryManager& rhs);
	GeometryManager& operator =(const GeometryManager& rhs);
	std::vector<uint> VAOs;
public:

	~GeometryManager(void);

	static GeometryManager& getInstance() { return instance; }

	GeometryInfo* getGeometry(uint index);
	uint getVAO(uint index);

	//all of these functions return an index into the geometryInfo array

	///Adds the geometry using the default layoutMager which is set
	////to match Neumont Vertex
	uint addGeometry(const void* vertexData,const void* indexData, uint vertexBufferSize,
		uint indexBufferSize, uint numVerts,
		uint numIndicies,uint vertexPositionOffset = 0,
		uint vertexUVOffset = 0,uint vertexNormalOffset = 0,
		uint vertexColorOffset = 0, DrawType drawType = drawTriangle);
	uint addGeometry(const ShapeData& shapeData, DrawType drawType = drawTriangle);

	///Allows for the use of a custom layoutManager, the manager will be saved for later reuse
	uint addGeometry(const ShaderLayoutManager& layoutManager,const void* vertexData,const void* indexData, uint vertexBufferSize,
		uint indexBufferSize, uint numVerts,
		uint numIndicies,uint vertexPositionOffset = 0,
		uint vertexUVOffset = 0,uint vertexNormalOffset = 0,
		uint vertexColorOffset = 0, DrawType drawType = drawTriangle);
	uint addGeometry(const ShaderLayoutManager& layoutManager,const ShapeData& shapeData, DrawType drawType = drawTriangle);
	
	void removeGeometryInfo(uint index);
};

#define Geometry_Manager GeometryManager::getInstance()

#endif