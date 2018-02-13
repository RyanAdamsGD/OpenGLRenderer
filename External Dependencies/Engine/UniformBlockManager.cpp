#include "UniformBlockManager.h"
#include "GL\glew.h"

uint UniformBlockManager::nextBindingID = 0;
UniformBlockManager::UniformBlockManager(uint programID)
	:programID(programID)
{
}

uint UniformBlockManager::addUniformBlock(const char* uniformBlockName, const char* uniformStructName)
{
	uniformBlocks.push_back(new UniformBlockInfo(programID,uniformBlockName,uniformStructName,nextBindingID++));
	return uniformBlocks.size() -1;
}

UniformBlockManager::~UniformBlockManager(void)
{
	uint size = uniformBlocks.size();
	for(uint i=0;i<size;i++)
	{
		if(uniformBlocks[i] != 0)
		{
			delete uniformBlocks[i];
			uniformBlocks[i] = 0;
		}
	}
	uniformBlocks.clear();
}

void UniformBlockManager::activateBlocks()const
{
	uint size = uniformBlocks.size();
	for(uint i=0;i<size;i++)
		uniformBlocks[i]->activateUniformBlock();
}

uint UniformBlockManager::getUniformBlockID(const char* uniformBlockName)const
{
	uint size = uniformBlocks.size();
	for(uint i=0;i<size;i++)
	{
		if(strcmp(uniformBlocks[i]->getUniformBlockName(),uniformBlockName) == 0)
			return i;
	}
}