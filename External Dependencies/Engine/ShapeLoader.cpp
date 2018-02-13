#include "ShapeLoader.h"
#include "RenderManager.h"
#include <iostream>
#include <fstream>
#include "ShapeData.h"
#include "AnimationKeySet.h"
#include "PathingPoint.h"
#include "PathTree.h"
#include "Connection.h"
#include "Pathing.h"

bool ShapeLoader::loadFile(const char* fileName, ShapeData* shapeData, uint& count)
{
	// File structure
	// size of model data
	// numShapeData (unsigned int)
	// shapeDatas
	// verts
	// indicies
	// texture file name
	// normal map name
	// skeleton
	// animation data

	//_A*_
	//num paths
	//PathTree
	//connections

	bool success =false;
	count = 0;
	shapeData = NULL;

	std::ifstream File(fileName, std::ios::in | std::ios::binary);
	if(File.is_open())
	{
		//read off the file size
		char* buffer = (char*) malloc(4);
		File.read (buffer, 4);

		uint size = *reinterpret_cast<uint*>(buffer);
		free(buffer);
		//read the rest of the model data
		buffer = (char*) malloc(size);
		File.read(buffer,size - sizeof(uint));

		uint count = reinterpret_cast<uint*>(buffer)[0];
		shapeData = (ShapeData*) malloc(sizeof(ShapeData) * count);
		uint bufferHeaderOffset = sizeof(uint);
		shapeData = reinterpret_cast<ShapeData*>(buffer + bufferHeaderOffset);
		//re align the pointers for the shapedata
		for(int i=0;i<count;i++)
		{
			//read in the offsets for the pointers
			uint vertexOffset = *reinterpret_cast<uint*>(buffer + bufferHeaderOffset + (i * sizeof(ShapeData)));
			//skip 1 uint for the num verts
			uint indexOffset = *reinterpret_cast<uint*>(buffer + bufferHeaderOffset + (i * sizeof(ShapeData)) + sizeof(uint) * 2);
			//skip 1 uint for the num indicies
			uint textureOffset = *reinterpret_cast<uint*>(buffer + bufferHeaderOffset + (i * sizeof(ShapeData)) + sizeof(uint) * 4);
			uint normalMapOffset = *reinterpret_cast<uint*>(buffer + bufferHeaderOffset + (i * sizeof(ShapeData)) + sizeof(uint) * 5);
			uint skeletonOffset = *reinterpret_cast<uint*>(buffer + bufferHeaderOffset + (i * sizeof(ShapeData)) + sizeof(uint) * 6);
			//skip 1 uint for the num bones
			uint animationOffset = *reinterpret_cast<uint*>(buffer + bufferHeaderOffset + (i * sizeof(ShapeData)) + sizeof(uint) * 8);
			//set the pointers
			shapeData[i].verts = reinterpret_cast<Vertex*>(buffer + vertexOffset-bufferHeaderOffset);
			shapeData[i].indices = reinterpret_cast<ushort*>(buffer + indexOffset-bufferHeaderOffset);
			shapeData[i].textureFileName = reinterpret_cast<char*>(buffer + textureOffset-bufferHeaderOffset);
			shapeData[i].normalMapFileName = reinterpret_cast<char*>(buffer + normalMapOffset-bufferHeaderOffset);
			//check if there is any skeleton data
			if(skeletonOffset != animationOffset)
			{
				shapeData[i].skeleton = reinterpret_cast<skeletonBone*>(buffer + skeletonOffset-bufferHeaderOffset);
			}
			else
			{
				shapeData[i].skeleton = NULL;
			}
			//check if there is any animation data
			if(animationOffset != *reinterpret_cast<uint*>(buffer + bufferHeaderOffset + ((i+1) * sizeof(ShapeData))))
			{
				shapeData[i].animation = reinterpret_cast<AnimationKeyFrameSet*>(buffer + animationOffset-bufferHeaderOffset);
			}
			else
			{
				shapeData[i].animation = NULL;
			}
		}	
		free(buffer);


		//TODO::load this into passed in variables rather than directly into pathing
		int c = File.peek();
		if(c != EOF)
		{
			buffer = (char*) malloc(4);
			File.read(buffer,4);

			if(strncmp(buffer,"_A*_",4) == 0)
			{
				free(buffer);
				buffer = (char*) malloc(sizeof(uint) * 2);
				File.read(buffer,sizeof(uint)*2);
				uint pathSize = *reinterpret_cast<uint*>(buffer);
				uint connectionSize = *reinterpret_cast<uint*>(buffer + sizeof(uint));
				free(buffer);


				buffer = (char*) malloc(pathSize + connectionSize);
				File.read(buffer,pathSize + connectionSize);
				uint jmpSize = sizeof(PathTree);
				PathTree* path = reinterpret_cast<PathTree*>(buffer);

				uint connectionIndex = 0;

				for(uint i=0;i<pathSize/jmpSize;i++)
				{
					//set up connections
					path[i].addConnections(reinterpret_cast<Connection*>(buffer + pathSize + sizeof(Connection) * connectionIndex));
					connectionIndex += path[i].numOfConnections;
				}
				Pathing::addPath(path,pathSize/jmpSize);
				free(buffer);
			}
		}
		File.clear();

		//clean up after myself
		File.close();
		success=true;
	}

	return success;
}