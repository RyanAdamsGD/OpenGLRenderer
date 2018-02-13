#ifndef DECISIONNODE_H
#define DECISIONNODE_H
#include "FastDelegate.h"
#include "MyTypeDefs.h"
#include "ExportHeader.h"
class State;

class ENGINE_SHARED DecisionNode
{
protected:
	DecisionNode** nodes;
	fastdelegate::FastDelegate0<uint> checkCondition;
	fastdelegate::FastDelegate0<State*>* returnState;
public:
	State* evaluate()const;
	DecisionNode(DecisionNode** nodes, fastdelegate::FastDelegate0<uint> checkCondition);
	DecisionNode(DecisionNode** nodes, fastdelegate::FastDelegate0<uint> checkCondition,
		fastdelegate::FastDelegate0<State*>* returnState);
	~DecisionNode();
	void cleanUp();
};

//EXAMPLE OF POSSIBLE USE
/*
childNodes = (DecisionNode**) malloc(sizeof(DecisionNode*) * 2);
returnState = (fastdelegate::FastDelegate0<State*>*) malloc(sizeof(fastdelegate::FastDelegate0<State*>*) * 2);
childNodes[0] = NULL;
childNodes[1] = &Node;
returnState[0] = fastdelegate::MakeDelegate(this,&Class::ReturnState);
returnState[1] = NULL;
DecisionNode(childNodes,fastdelegate::MakeDelegate(this,&CaptureTheFlag::ourTeamHasFlag),returnState);
*/

#endif