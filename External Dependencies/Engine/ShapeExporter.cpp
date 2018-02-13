#include "ShapeExporter.h"
#include "ShapeData.h"
#include <fstream>
#include <iostream>
#include <string>
#include "Helper Functions.h"

ShapeExporter ShapeExporter::instance;

ShapeExporter::ShapeExporter(void)
{
}

void ShapeExporter::writeShapeData(ShapeData* data, uint numOfShapeData, const char* fileName)
{
	char* buffer = NULL;

	//Create the buffer
	calculateDataSize(data,numOfShapeData);
	buffer = new char[totalFileSize];
	vertOffsets = new uint[numOfShapeData];
	indexOffsets = new uint[numOfShapeData];
	textureOffsets = new uint[numOfShapeData];
	normalMapOffsets = new uint[numOfShapeData];
	skeletonOffsets = new uint[numOfShapeData];
	animationOffsets = new uint[numOfShapeData];

	//skip over the header and insert data since we need to know the offsets first
	currentOffset=headerSize;
	//insert the data
	for(int i=0;i<numOfShapeData;i++)
	{
		ShapeData& where = data[i];
		convertVertData(where,buffer,i);
		convertIndexData(where,buffer,i);
		convertTextureData(where,buffer,i);
		convertNormalMapData(where,buffer,i);
		convertSkeletonData(where,buffer,i);
		convertAnimationData(where,buffer,i);
	}
	
	//create and insert the header
	createHeader(data,numOfShapeData,buffer);
	
	//clean up after myself
	for(int i=0;i<numOfShapeData;i++)
	{
		data[i].cleanUp();
	}

	//write to the file here
	writeToFile(getFileName(),buffer);


	//clean up some more
	if(buffer != NULL)
	{
		delete [] buffer;
		buffer = 0;
	}
}

void ShapeExporter::createHeader(const ShapeData* data,const uint& size,char* buffer)
{
	char* header = new char[headerSize];
	memcpy(&header[0],&totalFileSize,sizeof(uint));	
	memcpy(&header[4],&size,sizeof(uint));

	for(int i=0;i<size;i++)
	{
		memcpy(&header[i*sizeof(ShapeData)+8],&vertOffsets[i],sizeof(uint));
		memcpy(&header[i*sizeof(ShapeData)+12] ,&data[i].numVerts,sizeof(uint));
		memcpy(&header[i*sizeof(ShapeData)+16] ,&indexOffsets[i],sizeof(uint));
		memcpy(&header[i*sizeof(ShapeData)+20] ,&data[i].numIndices,sizeof(uint));
		memcpy(&header[i*sizeof(ShapeData)+24] ,&textureOffsets[i],sizeof(uint));
		memcpy(&header[i*sizeof(ShapeData)+28] ,&normalMapOffsets[i],sizeof(uint));
		memcpy(&header[i*sizeof(ShapeData)+32] ,&skeletonOffsets[i],sizeof(uint));
		memcpy(&header[i*sizeof(ShapeData)+36] ,&data[i].numBones,sizeof(uint));
		memcpy(&header[i*sizeof(ShapeData)+40] ,&animationOffsets[i],sizeof(uint));
		memcpy(&header[i*sizeof(ShapeData)+44], &data[i].numAnimations,sizeof(uint));
		memcpy(&header[i*sizeof(ShapeData)+48], &data[i].frameCount,sizeof(uint));
	}
	memcpy(&buffer[0],header,headerSize);
}


void ShapeExporter::calculateDataSize(const ShapeData* data,const uint& size)
{
	headerSize=0;
	totalFileSize=0;
	headerSize +=sizeof(unsigned int) *2;
	headerSize += sizeof(ShapeData) * size;

	totalFileSize+=headerSize;
	for(int i=0;i<size;i++)
	{
		const ShapeData& where = data[i];
		totalFileSize+= where.vertexBufferSize();
		totalFileSize+= where.indexBufferSize();
		if(where.textureFileName != NULL)
		{
			totalFileSize+= strlen(where.textureFileName)+1;
		}
		else
		{
			totalFileSize+=1;
		}
		if(where.normalMapFileName != NULL)
		{
			totalFileSize+= strlen(where.normalMapFileName)+1;
		}
		else
		{
			totalFileSize+=1;
		}
		totalFileSize+= where.animationBufferSize();
		totalFileSize+= where.numBones * sizeof(skeletonBone);
	}
}


void ShapeExporter::convertVertData(const ShapeData data,char* buffer,const uint& index)
{
	vertOffsets[index] = currentOffset;
	memcpy(&buffer[currentOffset],data.verts,data.vertexBufferSize());
	currentOffset += data.vertexBufferSize();
}


void ShapeExporter::convertIndexData(const ShapeData data,char* buffer,const uint& index)
{
	indexOffsets[index] = currentOffset;
	memcpy(&buffer[currentOffset],data.indices,data.indexBufferSize());
	currentOffset += data.indexBufferSize();
}

void ShapeExporter::convertTextureData(const ShapeData data,char* buffer,const uint& index)
{
	textureOffsets[index] = currentOffset;
	if(data.textureFileName != NULL)
	{
		memcpy(&buffer[currentOffset],data.textureFileName,strlen(data.textureFileName)+1);
		currentOffset += strlen(data.textureFileName)+1;
	}
	else
	{
		buffer[currentOffset] = '\0';
		currentOffset++;
	}
}
void ShapeExporter::convertNormalMapData(const ShapeData data,char* buffer,const uint& index)
{
	normalMapOffsets[index] = currentOffset;
	if(data.normalMapFileName != NULL)
	{
		memcpy(&buffer[currentOffset],data.normalMapFileName,strlen(data.normalMapFileName)+1);
		currentOffset += data.vertexBufferSize();
	}
	else
	{
		buffer[currentOffset] = '\0';
		currentOffset++;
	}
}
void ShapeExporter::convertSkeletonData(const ShapeData data,char* buffer,const uint& index)
{
	skeletonOffsets[index] = currentOffset;
	memcpy(&buffer[currentOffset],data.skeleton,data.numBones * sizeof(skeletonBone));
	currentOffset += data.numBones * sizeof(skeletonBone);
}
void ShapeExporter::convertAnimationData(const ShapeData data,char* buffer,const uint& index)
{
	animationOffsets[index] = currentOffset;
	for(int i=0;i<data.numAnimations;i++)
	{
		memcpy(&buffer[currentOffset],&data.animation[i].index,sizeof(int));
		currentOffset += sizeof(int);

		TimeKey* temp = toArray(data.animation[i].transKeys);
		memcpy(&buffer[currentOffset],temp,sizeof(TimeKey) * data.animation[i].transKeys.size());
		currentOffset += sizeof(TimeKey) * data.animation[i].transKeys.size();
		temp = toArray(data.animation[i].scaleKeys);
		memcpy(&buffer[currentOffset],temp,sizeof(TimeKey) * data.animation[i].scaleKeys.size());
		currentOffset += sizeof(TimeKey) * data.animation[i].scaleKeys.size();
		temp = toArray(data.animation[i].rotKeys);
		memcpy(&buffer[currentOffset],temp,sizeof(TimeKey) * data.animation[i].rotKeys.size());
		currentOffset += sizeof(TimeKey) * data.animation[i].rotKeys.size();
		TimeKeyFrame* temp1 = toArray(data.animation[i].quatKeys);
		memcpy(&buffer[currentOffset],temp1,sizeof(TimeKeyFrame) * data.animation[i].quatKeys.size());
		currentOffset += sizeof(TimeKeyFrame) * data.animation[i].quatKeys.size();
	}
}

void ShapeExporter::writeToFile(const char* filename, const char* buffer)
{
	if(buffer != NULL)
	{
		std::ofstream File(filename,std::ios::out | std::ios::binary);
		File.clear();
		File.write(buffer,totalFileSize);
		File.close();
	}
}

char* ShapeExporter::getFileName()const 
{
	char* fileName = new char[14];
	int existingFileCount = 0;
	while(true)
	{
		sprintf(fileName,"waterModel%04d.rbn",existingFileCount);
		std::ifstream file(fileName,std::ios::binary);
		//look until we find an open file name
		if(!file || existingFileCount > 9999)
			break;
		existingFileCount++;
	}
	return fileName;
}