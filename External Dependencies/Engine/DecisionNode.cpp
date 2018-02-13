#include "DecisionNode.h"
#include <memory>

DecisionNode::DecisionNode(DecisionNode** nodes, fastdelegate::FastDelegate0<uint> checkCondition)
	:nodes(nodes),checkCondition(checkCondition),returnState(NULL)
{
}
DecisionNode::DecisionNode(DecisionNode** nodes, fastdelegate::FastDelegate0<uint> checkCondition,fastdelegate::FastDelegate0<State*>* returnState)
	:nodes(nodes),checkCondition(checkCondition),returnState(returnState)
{
}

State* DecisionNode::evaluate()const
{
	uint index = checkCondition();
	//make sure our ** is not null
	if(nodes)
	{
		//see if the index is null
		if(nodes[index])
		{
			return nodes[index]->evaluate();
		}
		else
		{
			return returnState[index]();
		}
	}
	else
	{
		return returnState[index]();
	}
}

DecisionNode::~DecisionNode()
{	
}

void DecisionNode::cleanUp()
{
	free(returnState);
	free(nodes);
}