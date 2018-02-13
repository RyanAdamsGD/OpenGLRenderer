#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H
#include "glm\glm.hpp"
#include "ExportHeader.h"
#include "MyTypeDefs.h"

class ENGINE_SHARED TextRenderer
{
	int scaler;
	struct Sample
	{
		ushort x,y,width, height,xoffset,yoffset,offset;
	};
	//there is 95 letters in this bitmap
	Sample samples[95];

	struct Vert
	{
		glm::vec3 position;
		glm::vec2 UV;
	};

	struct TextID
	{
		uint bufferOffset;
		uint vertexBufferID;
		glm::vec2 position;
		float scale;
		uint stringLength;
	};
	const static uint MAX_NUM_TEXTID = 100;
	TextID strings[MAX_NUM_TEXTID];
	uint nextTextID;

	struct BufferInfo
	{
		uint bufferID;
		uint currentOffset;
	};
	const static uint MAX_NUM_BUFFERS = 50;
	BufferInfo bufferList[MAX_NUM_BUFFERS];
	uint nextBufferList;

	ushort baseHeight;
	ushort baseWidth;
	ushort fileWidth;
	ushort fileHeight;
	uint programID;
	uint indexID;
	uint vertexID;
	uint numIndicies;
	uint textureID;
	uint indexBufferID;
	uint fragShaderID;
	uint vertexShaderID;

	TextRenderer(void){}
	TextRenderer(const TextRenderer&){}

	static Vert* createTextBuffer(const char* text);
	static Sample getLetter(uint asciiValue);	
	static void createBuffers(uint vertexBufferSize, void* verts, uint indexBufferSize, ushort* indicies);
	static TextRenderer textManager;
	static void loadTexture();
	static void createProgram();
	static bool compileShader(uint shaderID, const char* fileName);
public:
	static void TextRenderer::updateText(const uint& ID, const char* text);
	///set the max buffer size if you wish to alter the data dynamically
	static uint addText(const char* text, const glm::vec2& position, const float scale =1.0f, const uint maxStringSize=0);	
	static void init();
	static void drawText();
	static void cleanup();
};

#endif