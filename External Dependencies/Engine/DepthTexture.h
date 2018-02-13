#ifndef DEPTH_TEXTURE_H
#define DEPTH_TEXTURE_H
#include "Texture.h"
#include "ExportHeader.h"

class ENGINE_SHARED DepthTexture: public Texture
{
public:
	DepthTexture(uint width, uint height);
	~DepthTexture(void);

	virtual void bindToFrameBuffer()const;
	virtual void bindTexture()const;
	virtual bool saveToFile(const char* fileName) const;

	virtual void setWidth(uint value);
	virtual void setHeight(uint value);

	virtual Texture* clone()const { return new DepthTexture(*this); }
};

#endif