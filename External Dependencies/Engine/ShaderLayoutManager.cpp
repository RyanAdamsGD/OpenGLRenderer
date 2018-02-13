#include "ShaderLayoutManager.h"

ShaderLayoutManager::ShaderLayoutManager(void)
{
}

ShaderLayoutManager::~ShaderLayoutManager(void)
{
	layoutCallbackInfos.clear();
}

void ShaderLayoutManager::addLayout(uint numberOfDataType, uint type, uint stride, uint offset)
{
	layoutCallbackInfos.push_back(LayoutCallbackInfo(numberOfDataType,type,stride,offset));
}

void ShaderLayoutManager::activateLayouts(uint* attribArrayLocations, uint bufferOffset) const
{
	uint size = layoutCallbackInfos.size();
	for(uint i=0;i<size;i++)
	{
		layoutCallbackInfos[i].activateLayout(attribArrayLocations[i],bufferOffset);
	}
}

void ShaderLayoutManager::activateLayouts(uint bufferOffset) const
{
	uint size = layoutCallbackInfos.size();
	for(uint i=0;i<size;i++)
	{
		layoutCallbackInfos[i].activateLayout(i,bufferOffset);
	}
}