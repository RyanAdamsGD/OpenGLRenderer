#ifndef SHAPELOADER_H
#define SHAPELOADER_H
#include "ExportHeader.h"
#include "MyTypeDefs.h"
class ShapeData;

class ENGINE_SHARED ShapeLoader
{	
public:
	static bool loadFile(const char* fileName, ShapeData* shapeData, uint& count);
};

#endif