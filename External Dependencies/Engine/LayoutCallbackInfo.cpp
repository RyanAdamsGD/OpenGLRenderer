#include "LayoutCallbackInfo.h"
#include <GL\glew.h>

LayoutCallbackInfo::LayoutCallbackInfo(uint numberOfDataType, uint type, uint stride,uint offset)
	:numberOfDataType(numberOfDataType),type(type),stride(stride),offset(offset)
{
}


LayoutCallbackInfo::~LayoutCallbackInfo(void)
{
}

void LayoutCallbackInfo::activateLayout(uint attribArrayLocation,uint bufferOffset) const
{
	glEnableVertexAttribArray(attribArrayLocation);
	glVertexAttribPointer(attribArrayLocation,numberOfDataType,GL_FLOAT,GL_FALSE,stride,(void*)(bufferOffset + offset));
}
