#ifndef TEXTURE_H
#define TEXTURE_H
#include "MyTypeDefs.h"
#include "FastDelegate.h"
#include <glm\gtx\transform.hpp>
#include "ExportHeader.h"

class ENGINE_SHARED Texture
{
protected:
	//this is mutable so we can copy it properly without
	//the texture being deleted by the destructor
	mutable uint textureID;
	uint width;
	uint height;
	bool isDynamic;

	static uint textureFrameBufferID;
public:
	Texture(uint textureID, uint width = 0, uint height = 0);
	Texture(void);
	///these assume the value being copy will not be used again
	Texture(const Texture& rhs);
	Texture& operator =(const Texture& rhs);
	~Texture(void);
	static void setTextureFrameBufferID(uint ID);

	//TODO: this delegate may need to be changed with the new standard for drawing
	virtual void updateTexture(const fastdelegate::FastDelegate2<const glm::mat4&,const glm::mat4&>& drawFunction)const;
	virtual void bindToFrameBuffer()const {}
	virtual void bindTexture()const = 0;
	virtual bool saveToFile(const char* fileName) const;

	virtual void setTextureID(uint value);
	virtual void setWidth(uint value) { width = value; }
	virtual void setHeight(uint value) { height = value; }

	inline uint getTextureID() const { return textureID; }
	inline virtual const char* getFileName() const { return ""; }
	inline static uint getTextureFrameBufferID() { return textureFrameBufferID; }

	virtual Texture* clone()const = 0;
};

#endif