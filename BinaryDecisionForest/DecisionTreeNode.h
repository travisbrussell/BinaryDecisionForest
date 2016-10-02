#pragma once
#include "Question.h"

struct DecisionTreeNode
{
	DecisionTreeNode()
	{
		// constructor
	}

	DecisionTreeNode(int level)
	{
		Level = level;
	}

	~DecisionTreeNode()
	{
		delete Question;
		delete LeftChild;
		delete RightChild;
	}

	Question* Question;
	DecisionTreeNode* LeftChild;
	DecisionTreeNode* RightChild;
	// should be a non-negative integer
	short NodeClass;
	int Level;
};