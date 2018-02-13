#include "TextRenderer.h"
#include "GL\glew.h"
#include <iostream>
#include <Qt\qfile.h>
#include <Qt\qtextstream.h>
#include <Qt\qdebug.h>
#include <glm\gtx\transform.hpp>
#include <Qt\qimage.h>
#include <QtOpenGL\qglwidget>
#include "LuaState.h"

#ifdef _DEBUG
TextRenderer TextRenderer::textManager;
#endif

void TextRenderer::init()
{
	LuaState luaState("../../External Dependencies/Engine/config.lua");
	textManager.scaler = luaState.get<int>("TextScaler");
	//these are all numbers pulled from the text file
	//created when creating the bitmap for the font
	textManager.baseHeight = luaState.get<int>("TextbaseHeight");
	textManager.baseWidth = luaState.get<int>("TextbaseWidth");
	textManager.fileWidth = luaState.get<int>("TextfileWidth");
	textManager.fileHeight = luaState.get<int>("TextfileHeight");
	Sample temp[] = 
	{
		511   ,0     ,0     ,1     ,0     ,91,	23,
		478   ,247   ,10    ,60    ,7     ,14,	23,
		353   ,366   ,22    ,22    ,3     ,14,	29,
		157   ,80    ,45    ,62    ,0     ,13,	46,
		232   ,0     ,40    ,73    ,2     ,10,	46,
		333   ,0     ,64    ,64    ,4     ,13,	73,
		57    ,80    ,50    ,62    ,3     ,13,	55,
		376   ,358   ,9     ,22    ,3     ,14,	15,
		128   ,0     ,20    ,79    ,4     ,13,	27,
		149   ,0     ,20    ,79    ,5     ,13,	27,
		283   ,372   ,28    ,26    ,2     ,13,	32,
		205   ,373   ,40    ,40    ,4     ,25,	48,
		386   ,358   ,10    ,20    ,6     ,66,	23,
		457   ,356   ,23    ,8     ,2     ,49,	27,
		481   ,356   ,9     ,8     ,7     ,66,	23,
		243   ,74    ,24    ,62    ,0     ,13,	23,
		40    ,143   ,39    ,61    ,3     ,14,	46,
		478   ,186   ,23    ,60    ,8     ,14,	46,
		361   ,252   ,40    ,60    ,2     ,14,	46,
		80    ,143   ,39    ,61    ,3     ,14,	46,
		236   ,266   ,42    ,60    ,0     ,14,	46,
		279   ,260   ,40    ,60    ,3     ,15,	46,
		341   ,65    ,40    ,61    ,3     ,14,	46,
		0     ,327   ,39    ,59    ,3     ,15,	46,
		423   ,63    ,40    ,61    ,3     ,14,	46,
		464   ,63    ,40    ,61    ,3     ,14,	46,
		493   ,308   ,9     ,43    ,7     ,31,	23,
		63    ,327   ,10    ,55    ,6     ,31,	23,
		164   ,373   ,40    ,40    ,4     ,25,	48,
		312   ,372   ,40    ,25    ,4     ,32,	48,
		123   ,373   ,40    ,40    ,4     ,25,	48,
		0     ,143   ,39    ,61    ,3     ,13,	46,
		0     ,0     ,77    ,79    ,4     ,13,	84,
		0     ,205   ,56    ,60    ,0     ,14,	55,
		96    ,266   ,46    ,60    ,5     ,14,	55,
		457   ,0     ,53    ,62    ,4     ,13,	59,
		278   ,199   ,50    ,60    ,6     ,14,	59,
		190   ,266   ,45    ,60    ,6     ,14,	55,
		320   ,260   ,40    ,60    ,6     ,14,	50,
		0     ,80    ,56    ,62    ,4     ,13,	64,
		0     ,266   ,47    ,60    ,6     ,14,	59,
		502   ,186   ,9     ,60    ,7     ,14,	23,
		276   ,137   ,34    ,61    ,2     ,14,	41,
		329   ,191   ,50    ,60    ,5     ,14,	55,
		402   ,251   ,38    ,60    ,5     ,14,	46,
		416   ,127   ,57    ,60    ,6     ,14,	69,
		48    ,266   ,47    ,60    ,6     ,14,	59,
		398   ,0     ,58    ,62    ,3     ,13,	64,
		143   ,266   ,46    ,60    ,6     ,14,	55,
		273   ,0     ,59    ,66    ,3     ,13,	64,
		224   ,205   ,53    ,60    ,6     ,14,	59,
		108   ,80    ,48    ,62    ,3     ,13,	55,
		429   ,188   ,48    ,60    ,1     ,14,	50,
		293   ,67    ,47    ,61    ,6     ,14,	59,
		113   ,205   ,55    ,60    ,0     ,14,	55,
		338   ,129   ,77    ,60    ,0     ,14,	78,
		169   ,205   ,54    ,60    ,0     ,14,	55,
		57    ,205   ,55    ,60    ,0     ,14,	55,
		380   ,190   ,48    ,60    ,1     ,14,	50,
		196   ,0     ,17    ,77    ,5     ,14,	23,
		268   ,74    ,24    ,62    ,0     ,13,	23,
		214   ,0     ,17    ,77    ,1     ,14,	23,
		246   ,373   ,36    ,33    ,2     ,13,	38,
		83    ,417   ,49    ,6     ,-1    ,85,	46,
		440   ,356   ,16    ,12    ,3     ,14,	27,
		116   ,327   ,40    ,45    ,2     ,30,	46,
		237   ,141   ,38    ,61    ,5     ,14,	46,
		198   ,327   ,38    ,45    ,3     ,30,	41,
		198   ,143   ,38    ,61    ,2     ,14,	46,
		157   ,327   ,40    ,45    ,2     ,30,	46,
		311   ,129   ,26    ,61    ,0     ,13,	23,
		203   ,78    ,39    ,62    ,2     ,30,	46,
		474   ,125   ,36    ,60    ,5     ,14,	46,
		489   ,247   ,8     ,60    ,5     ,14,	18,
		178   ,0     ,17    ,78    ,-4    ,14,	18,
		441   ,249   ,36    ,60    ,5     ,14,	41,
		498   ,247   ,8     ,60    ,5     ,14,	18,
		274   ,327   ,59    ,44    ,5     ,30,	69,
		334   ,321   ,36    ,44    ,5     ,30,	46,
		74    ,327   ,41    ,45    ,2     ,30,	46,
		120   ,143   ,38    ,61    ,5     ,30,	46,
		159   ,143   ,38    ,61    ,2     ,30,	46,
		408   ,312   ,24    ,44    ,5     ,30,	27,
		237   ,327   ,36    ,45    ,2     ,30,	41,
		40    ,327   ,22    ,59    ,1     ,16,	23,
		371   ,313   ,36    ,44    ,5     ,31,	46,
		42    ,387   ,40    ,43    ,1     ,31,	41,
		433   ,312   ,59    ,43    ,0     ,31,	59,
		0     ,387   ,41    ,43    ,0     ,31,	41,
		382   ,65    ,40    ,61    ,1     ,31,	41,
		83    ,373   ,39    ,43    ,1     ,31,	41,
		78    ,0     ,24    ,79    ,2     ,13,	27,
		170   ,0     ,7     ,79    ,7     ,13,	21,
		103   ,0     ,24    ,79    ,1     ,13,	27,
		397   ,358   ,42    ,14    ,3     ,38,	48
	};
	for(int i=0;i<95;i++)
	{
		textManager.samples[i] = temp[i];
	}

	textManager.indexBufferID = -1;
	textManager.nextBufferList =0;
	textManager.nextTextID = 0;
	loadTexture();
	createProgram();
}

void TextRenderer::loadTexture()
{
	QImage timg =
		QGLWidget::convertToGLFormat(QImage("text_0.png","PNG"));
	glGenTextures(1, &textManager.textureID);
	glBindTexture(GL_TEXTURE_2D, textManager.textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timg.width(),
		timg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		timg.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
}


void TextRenderer::createBuffers(uint vertexBufferSize, void* verts, uint indexBufferSize, ushort* indicies)
{
	//vertex buffer
	glGenBuffers(1,&textManager.vertexID);
	glBindBuffer(GL_ARRAY_BUFFER,textManager.vertexID);
	glBufferData(GL_ARRAY_BUFFER,vertexBufferSize,verts,GL_STATIC_DRAW);

	//index buffer
	glGenBuffers(1,&textManager.indexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,textManager.indexID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexBufferSize,indicies,GL_STATIC_COPY);
}

void TextRenderer::createProgram()
{
	textManager.vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	textManager.fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	textManager.programID = glCreateProgram();
	compileShader(textManager.vertexShaderID, "MyVertexShaderText.txt");
	compileShader(textManager.fragShaderID,"FragShaderText.txt");
	glAttachShader(textManager.programID,textManager.vertexShaderID);
	glAttachShader(textManager.programID,textManager.fragShaderID);
	glLinkProgram(textManager.programID);
}

TextRenderer::Vert* TextRenderer::createTextBuffer(const char* text)
{
	float currentOffset =0;
	Vert *verts = (Vert*) malloc(sizeof(Vert) * 4 * strlen(text));
	for(int i=0;i<strlen(text);i++)
	{
		Sample character= getLetter(text[i]);

		//bot left
		verts[i*4].UV.x = (float)character.x / textManager.fileWidth;
		verts[i*4].UV.y = float(textManager.fileHeight - character.y - character.height)/ textManager.fileHeight;
		verts[i*4].position.x = (float)currentOffset + character.xoffset;
		verts[i*4].position.y = (float)character.height + character.yoffset;
		verts[i*4].position.z = 0.0f;

		//bot right
		verts[i*4 +1].UV.x = (float)(character.x + character.width) / textManager.fileWidth;
		verts[i*4 +1].UV.y = float(textManager.fileHeight - character.y - character.height)/ textManager.fileHeight;
		verts[i*4 +1].position.x = (float)character.width + currentOffset + character.xoffset;
		verts[i*4 +1].position.y = (float)character.height + character.yoffset;
		verts[i*4 +1].position.z = 0.0f;

		//top right
		verts[i*4 +2].UV.x = (float)(character.x + character.width) / textManager.fileWidth;
		verts[i*4 +2].UV.y = (float(textManager.fileHeight - character.y - character.height) + character.height) / textManager.fileHeight;
		verts[i*4 +2].position.x = (float)character.width + currentOffset + character.xoffset;
		verts[i*4 +2].position.y = (float)character.yoffset;
		verts[i*4 +2].position.z = 0.0f;

		//top left
		verts[i*4 +3].UV.x = (float)character.x / textManager.fileWidth;
		verts[i*4 +3].UV.y = (float(textManager.fileHeight - character.y - character.height) + character.height) / textManager.fileHeight;
		verts[i*4 +3].position.x = (float)currentOffset + character.xoffset;
		verts[i*4 +3].position.y = (float)character.yoffset;
		verts[i*4 +3].position.z = 0.0f;
		
		currentOffset += character.offset;
	}
	return verts;
}


uint TextRenderer::addText(const char* text, const glm::vec2& position, const float scale, const uint maxStringSize)
{
	//assume the string coming down is smaller than a kilobyte
	assert(strlen(text)<KILOBYTE);

	bool vertexAdded = false;
	TextID& where = textManager.strings[textManager.nextTextID];
	where.scale = scale;
	where.position = position;
	where.stringLength = strlen(text);
	uint vertexBufferSize = where.stringLength * sizeof(Vert) * 4;
	Vert* vertexData = createTextBuffer(text);
	
	//setup a buffer for dynamic alteration
	if(maxStringSize != 0)
	{
		where.bufferOffset = 0;
		glGenBuffers(1,&where.vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER,where.vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER,maxStringSize * sizeof(Vert) * 4,NULL,GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER,0,vertexBufferSize,vertexData);
		textManager.bufferList[textManager.nextBufferList].bufferID = where.vertexBufferID;
		textManager.bufferList[textManager.nextBufferList].currentOffset = 0;
		textManager.nextBufferList++;
		vertexAdded = true;
	}

	//loop through all the current buffers to look for open spaces to place the data
	for(int i=0;i<textManager.nextBufferList;i++)
	{
		//check if we have already added the vertex data and check if there is room at the end of the buffer
		if(!vertexAdded && textManager.bufferList[i].currentOffset + vertexBufferSize < KILOBYTE * sizeof(Vert) * 4)
		{
			where.vertexBufferID = textManager.bufferList[i].bufferID;
			where.bufferOffset = textManager.bufferList[i].currentOffset;

			glBindBuffer(GL_ARRAY_BUFFER,where.vertexBufferID);
			glBufferSubData(GL_ARRAY_BUFFER,where.bufferOffset,vertexBufferSize,vertexData);
			textManager.bufferList[i].currentOffset += vertexBufferSize;
			vertexAdded=true;		
		}
	}

	//if we werent able to find a buffer that had enough room for the
	//vertex data we create a new one
	if(!vertexAdded)
	{
		glGenBuffers(1,&where.vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER,where.vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER,KILOBYTE * sizeof(Vert) * 4,NULL,GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER,0,vertexBufferSize,vertexData);
		textManager.bufferList[textManager.nextBufferList].currentOffset =vertexBufferSize;
		textManager.bufferList[textManager.nextBufferList].bufferID= where.vertexBufferID;
		textManager.nextBufferList++;
	}

	//create the index buffer if we haven't yet
	if(textManager.indexBufferID == -1)
	{
		//create the indicies
		ushort* ind = (ushort*) malloc(sizeof(ushort) * 6 * KILOBYTE);
		for(int i=0;i<KILOBYTE;i++)
		{
			ind[i*6] = i*4;
			ind[i*6+1] = i*4+1;
			ind[i*6+2] = i*4+2;
			ind[i*6+3] = i*4;
			ind[i*6+4] = i*4+2;
			ind[i*6+5] = i*4+3;
		}

		glGenBuffers(1,&textManager.indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,textManager.indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(ushort) * 6 * KILOBYTE,ind,GL_STATIC_DRAW);
		//clean up
		free(ind);
	}
	//clean up
	free(vertexData);

	return textManager.nextTextID++;
}

bool TextRenderer::compileShader(uint shaderID, const char* fileName)
{
	bool success=true;
	QFile input(fileName);
	if(!input.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<< "File failed to open: "<< fileName;
		success = false;
	}
	QTextStream stream(&input);
	QString shaderCodeText = stream.readAll();
	input.close();
	std::string temp = shaderCodeText.toStdString();
	const char *shaderSource = temp.c_str();

	const char* buf[1];
	buf[0] = shaderSource;
	glShaderSource(shaderID,1,buf,NULL);
	glCompileShader(shaderID);

	GLint status;
	glGetShaderiv(shaderID,GL_COMPILE_STATUS,&status);
	if(status != GL_TRUE)
	{
		//TODO:
		//Log this
		GLint infoLogLength;
		glGetShaderiv(shaderID,GL_INFO_LOG_LENGTH,&infoLogLength);
		char* buffer = (char*) malloc(infoLogLength);
		GLsizei bitBucket;
		glGetShaderInfoLog(shaderID,infoLogLength,&bitBucket,buffer);
		free(buffer);
		success = false;
	}
	return success;
}

TextRenderer::Sample TextRenderer::getLetter(uint asciiValue)
{
	if(asciiValue>126 || asciiValue<32)
	{
		asciiValue=32;
	}
	
	return textManager.samples[asciiValue-32];
}

void TextRenderer::updateText(const uint& ID, const char* text)
{
	TextID& where = textManager.strings[ID];
	where.stringLength = strlen(text);
	uint vertexBufferSize = where.stringLength * sizeof(Vert) * 4;
	Vert* vertexData = createTextBuffer(text);

	glBindBuffer(GL_ARRAY_BUFFER,where.vertexBufferID);
	glBufferSubData(GL_ARRAY_BUFFER,where.bufferOffset,vertexBufferSize,vertexData);		
}

void TextRenderer::drawText()
{
	glDisable(GL_DEPTH_TEST);
	glUseProgram(textManager.programID);
	//always going to use the same index buffer to draw everything
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,textManager.indexBufferID);
	
	for(uint i=0;i<textManager.nextTextID;i++)
	{
		TextID& where = textManager.strings[i];
		float trueScale = where.scale/textManager.scaler;

		glBindBuffer(GL_ARRAY_BUFFER,where.vertexBufferID);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vert),BUFFER_OFFSET(where.bufferOffset));
		glEnableVertexAttribArray(1);				
		glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(Vert),BUFFER_OFFSET(sizeof(glm::vec3)+where.bufferOffset));

		//translate to the center of the text
		//flip the text right side up
		//scale it by my altered version of the scale
		//TODO
		//fix this translation
		//translate it to somewhat reasonable coordinates
		glm::mat4 xfm = glm::translate(where.position.x - 0.990f,where.position.y + 0.95f,0.0f)
			* glm::scale(trueScale, trueScale, 1.0f)
			* glm::scale(1.0f,-1.0f,1.0f)
			* glm::translate(0.0f,(float)textManager.baseHeight/-2.0f,0.0f);

		uint uniLocation = glGetUniformLocation(textManager.programID,"mvp");
		glUniformMatrix4fv(uniLocation,1,GL_FALSE,&xfm[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,textManager.textureID);

		uniLocation = glGetUniformLocation(textManager.programID,"baseTex");
		glUniform1i(uniLocation,0);

		glDrawElements(GL_TRIANGLES,where.stringLength * 6,GL_UNSIGNED_SHORT,BUFFER_OFFSET(0));
	}
	glEnable(GL_DEPTH_TEST);
}

void TextRenderer::cleanup()
{
	for(uint i=0;i<textManager.nextBufferList;i++)
	{
		glDeleteBuffers(1,&textManager.bufferList[i].bufferID);
	}
	glDeleteShader(textManager.fragShaderID);
	glDeleteShader(textManager.vertexShaderID);
	glDeleteProgram(textManager.programID);
}