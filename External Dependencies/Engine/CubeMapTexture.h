#ifndef CUBE_MAP_TEXTURE_H
#define CUBE_MAP_TEXTURE_H
#include "Texture.h"
#include <glm\gtx\transform.hpp>
#include "ExportHeader.h"
class DepthTexture;

class ENGINE_SHARED CubeMapTexture: public Texture
{
	const glm::vec3* position;
	DepthTexture* depthTexture;
	bool staticTexture;

	void updateTexelCount(uint texelCount);
public:
	CubeMapTexture(uint texelCount);
	~CubeMapTexture(void);

	///To be implemented
	virtual void bindTexture()const;
	virtual bool saveToFile(const char* fileName) const;

	virtual void setWidth(uint value); 
	virtual void setHeight(uint value);

	virtual void updateTexture(const fastdelegate::FastDelegate2<const glm::mat4&,const glm::mat4&>& drawFunction);
	
	virtual Texture* clone()const { return new CubeMapTexture(*this); }
};

#endif