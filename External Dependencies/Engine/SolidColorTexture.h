#ifndef SOLID_COLOR_TEXTURE
#define SOLID_COLOR_TEXTURE
#include "Texture.h"
#include "MyTypeDefs.h"
#include "glm\core\type_vec.hpp"

class SolidColorTexture: public Texture
{
	glm::vec4 color;
	void createTexture();
	uint colorVecToUint(const glm::vec4& color);
public:
	SolidColorTexture(const glm::vec4& color);
	~SolidColorTexture(void);

	virtual void bindToFrameBuffer()const;

	virtual void bindTexture()const;
	inline virtual const glm::vec4& getFileName() { return color; }

	virtual Texture* clone()const { return new SolidColorTexture(*this); }
	void setColor(const glm::vec4& value);
};

#endif