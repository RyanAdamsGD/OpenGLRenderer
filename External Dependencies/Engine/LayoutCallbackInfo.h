#ifndef LAYOUT_CALLBACK_INFO_H
#define LAYOUT_CALLBACK_INFO_H
#include "MyTypeDefs.h"

class LayoutCallbackInfo
{
	uint numberOfDataType;
	uint type;
	uint stride;
	uint offset;
			
public:
	LayoutCallbackInfo(uint numberOfDataType, uint type, uint stride,uint offset);
	~LayoutCallbackInfo(void);

	void activateLayout(uint attribArrayLocation,uint bufferOffset) const;
};

#endif