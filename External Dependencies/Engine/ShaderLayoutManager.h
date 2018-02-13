#ifndef SHADER_LAYOUT_MANAGER_H
#define SHADER_LAYOUT_MANAGER_H
#include "MyTypeDefs.h"
#include <vector>
#include "LayoutCallbackInfo.h"

class ShaderLayoutManager
{
	std::vector<LayoutCallbackInfo> layoutCallbackInfos;

public:
	ShaderLayoutManager(void);
	~ShaderLayoutManager(void);

	void addLayout(uint numberOfDataType, uint type, uint stride, uint offset);

	///this call assumes layouts were added in the same order they appear in the shader
	void activateLayouts(uint bufferOffset) const;
	///attribArrayLocations should be an array of equivalent or larger size
	///than the number of layout callbacks current added
	void activateLayouts(uint* attribArrayLocations, uint bufferOffset) const;
	
	uint getLayoutCount() const { return layoutCallbackInfos.size(); }
};

#endif