#ifndef FILE_TEXTURE_H
#define FILE_TEXTURE_H
#include "Texture.h"
#include "ExportHeader.h"

class ENGINE_SHARED FileTexture: public Texture
{
	const char* fileName;
	void FileTexture::loadTexture();
protected:
public:
	FileTexture(uint textureID, const char* fileName);
	FileTexture(const char* fileName);
	~FileTexture(void);

	virtual void bindToFrameBuffer()const;

	virtual void bindTexture()const;
	inline virtual const char* getFileName() { return fileName; }

	virtual Texture* clone()const { return new FileTexture(*this); }
};

#endif