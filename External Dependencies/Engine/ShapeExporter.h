#ifndef SHAPEEXPORTER_H
#define SHAPEEXPORTER_H
#include "ExportHeader.h"
#include "MyTypeDefs.h"
#include <vector>
class ShapeData;


class ENGINE_SHARED ShapeExporter
{
	uint headerSize;
	uint totalFileSize;
	uint currentOffset;
	uint* vertOffsets;
	uint* indexOffsets;
	uint* textureOffsets;
	uint* normalMapOffsets;
	uint* skeletonOffsets;
	uint* animationOffsets;

	void addFile(const char* filename);
	void createHeader(const ShapeData* data,const uint& size,char* buffer);
	void convertAnimationVectors(const ShapeData* data);
	void convertVertData(const ShapeData data,char* buffer,const uint& index);
	void convertIndexData(const ShapeData data,char* buffer,const uint& index);
	void convertTextureData(const ShapeData data,char* buffer,const uint& index);
	void convertNormalMapData(const ShapeData data,char* buffer,const uint& index);
	void convertSkeletonData(const ShapeData data,char* buffer,const uint& index);
	void convertAnimationData(const ShapeData data,char* buffer,const uint& index);
	void writeToFile(const char* filename, const char* buffer);
	void calculateDataSize(const ShapeData* data,const uint& size);

	char* ShapeExporter::getFileName()const;

	ShapeExporter(void);
	ShapeExporter(const ShapeExporter&);
	ShapeExporter& operator =(const ShapeExporter&);

	static ShapeExporter instance;
public:
	static ShapeExporter& getInstance() { return instance; }

	void writeShapeData(ShapeData* data, uint numOfShapeData, const char* fileName);
	
};

#define Shape_Exporter ShapeExporter::getInstance()

#endif