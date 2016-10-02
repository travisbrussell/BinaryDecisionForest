#pragma once

#include "DecisionTreeNode.h"
#include "DataHandler.h"
#include <math.h>

ref class DecisionTree
{
public:
	DecisionTree();
	DecisionTree(DataHandler* dataHandler, Config* config);
	void GenerateTree(DecisionTreeNode* node, DataHandler* dataHandler, Config* config);
	Question* ReturnBestQuestion(DataHandler* dataHandler, Config* config);
	double ReturnQuestionImpurity(Question* question, DataHandler* dataHandler, Config* config);
	short MostLikelyNodeClass(DataHandler* dataHandler);
	short ClassifyDataPoint(DataHandler* dataHandler, DataPoint* dataPoint, DecisionTreeNode* node);
	double NodeEntropy(DataHandler* dataHandler);
	~DecisionTree();
	DecisionTreeNode* Root;
};

