#include "UniformBlockInfo.h"
#include "GL\glew.h"
#include "Logger.h"

UniformBlockInfo::UniformBlockInfo(uint programID, const char* uniformArrayName, const char* uniformStructName, uint blockID)
	:bindPoint(blockID),name(uniformArrayName),blockCount(0),data(0)
{
	////////////Uniform block array constructor/////////////

	//TODO:: error checking on the locations I get back.
	//Bad locations can cause the allocations to explode.

	//get the actual uniform that this block refers to location
	location = glGetUniformBlockIndex(programID,uniformArrayName);
	//get the size of the array block
	int totalBlockSize;
	glGetActiveUniformBlockiv(programID,location,GL_UNIFORM_BLOCK_DATA_SIZE,&totalBlockSize);
	//allocate memory for the data to be stored
	data = new uchar[totalBlockSize];
	memset(data,0,totalBlockSize);

	//get the location of the uniform holding the correct offsets
	uint uniformIndex = glGetUniformBlockIndex(programID,uniformStructName);
	//figure out how many params there are
	//TODO:: Possibly save the numParams into the class then check that against the templated
	//function calls to make sure the user is passing in the correct amount of data
	glGetActiveUniformBlockiv(programID, uniformIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numParams);
	int* indices =  new int[numParams];
	//get the indices of all of the parameters
	glGetActiveUniformBlockiv(programID, uniformIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices);
	//convert them to UNSIGNED ints because...thank you opengl for this
	uint* uindices = new uint[numParams];
	for(uint i=0;i<numParams;i++)
		uindices[i] = indices[i];
	delete [] indices;

	//finally get the offsets for each parameter
	offsets = new int[numParams];
	glGetActiveUniformsiv(programID, numParams, uindices, GL_UNIFORM_OFFSET, offsets);
	delete [] uindices;
	glGetActiveUniformBlockiv(programID, uniformIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

	totalBlockCount = totalBlockSize / blockSize;
	//allocate memory for our dataPtrs
	dataPtrs = new uchar*[totalBlockCount];
	memset(dataPtrs,0,totalBlockCount * sizeof(uchar*));

#ifdef USE_LOGGER
	if(totalBlockSize % blockSize != 0)
	{
		char msg[256];
		sprintf(msg,"size of array does not match size of structs. Array: ");
		strcat(msg,uniformArrayName);
		strcat(msg,"  Struct: ");
		strcat(msg,uniformStructName);
		LOGE(msg)
	}
#endif

	glGenBuffers(1,&bufferID);
}


UniformBlockInfo::~UniformBlockInfo(void)
{
	if(offsets != 0)
	{
		delete [] offsets;
		offsets = 0;
	}
}

void UniformBlockInfo::bindBlock()
{
	glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
	glBufferData(GL_UNIFORM_BUFFER,blockSize * blockCount,data,GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, bindPoint, bufferID, 0, blockSize * blockCount);
}

void UniformBlockInfo::activateUniformBlock()const
{
	glUniformBlockBinding(programID, location, bindPoint);
}

uchar* UniformBlockInfo::addNewDataBlock(uint& index)
{
	if(totalBlockCount == blockCount)
	{
#ifdef USE_LOGGER
		char msg[256];
		sprintf(msg,"Attempt to add data to full uniform block array. Block Name: ");
		strcat(msg,name);
		LOGE(msg);
#endif
		//TODO: do error checking on the return so this doesn't cause and explosion
		//in the templates
		return 0;
	}
	index = -1;
	uchar* newDataBlockAddress = &data[blockSize * blockCount++];

	//search for the first open index
	for(uint i=0;i<totalBlockCount;i++)
	{
		//find the offset of the pointer and move it that far into the new array
		if(dataPtrs[i] != 0)
		{
			dataPtrs[i] = newDataBlockAddress;
			index = i;
			break;
		}
	}

#ifdef USE_LOGGER
	if(index == -1)
	{
		char msg[256];
		sprintf(msg,"Error occured in finding open data block. Block Name: ");
		strcat(msg,name);
		LOGE(msg);
	}
#endif
}

void UniformBlockInfo::removeData(uint index)
{
	if(index >= totalBlockCount)
	{
#ifdef USE_LOGGER
		char msg[256];
		sprintf(msg,"Attempt to remove data at invalid index. Index: %u   Block Name: ", index);
		strcat(msg,name);
		LOGE(msg);
#endif
		return;
	}

	//pull all of the data beyond the requested index forward
	memcpy(dataPtrs[index],dataPtrs[index] + blockSize,(totalBlockCount * blockSize) - (blockSize + dataPtrs[index] - data));
	//fix up the pointers to the moved data
	for(uint i=0;i<totalBlockCount;i++)
	{
		//move the pointer back a block if it pointed beyond the one we just erased
		if(dataPtrs[i] > dataPtrs[index])
			*dataPtrs[i] -= blockSize;
	}
	//clear out the last block in our data set since it may have previously contained data
	memset(&data[(totalBlockCount-1) * blockSize],0,blockSize);
	dataPtrs[index] = 0;
	blockCount--;
}
