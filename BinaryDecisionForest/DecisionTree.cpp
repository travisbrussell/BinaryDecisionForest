#include "stdafx.h"
#include "DecisionTree.h"

DecisionTree::DecisionTree()
{
	// default constructor
}

DecisionTree::DecisionTree(DataHandler* dataHandler, Config* config)
{
	// train decision tree
	Root = new DecisionTreeNode(1);
	GenerateTree(Root, dataHandler, config);
}

DecisionTree::~DecisionTree()
{
	//delete Root;
}

void DecisionTree::GenerateTree(DecisionTreeNode* node, DataHandler* dataHandler, Config* config)
{
	if (NodeEntropy(dataHandler) > config->MinEntropy && node->Level < config->MaxNumberOfLevels)
	{
		// set node class to -1 to indicate that this is not a leaf
		node->NodeClass = -1;

		// Find best question
		node->Question = ReturnBestQuestion(dataHandler, config);

		// count data split size
		int leftDataPoints = 0;
		int rightDataPoints = 0;
		for (int dataIndex = 0; dataIndex < dataHandler->NumberOfDataPoints; dataIndex++)
		{
			if (dataHandler->AskQuestion(node->Question, dataHandler->DataPoints[dataIndex]))
			{
				rightDataPoints++;
			}
			else
			{
				leftDataPoints++;
			}
		}

		// partition left and right data
		DataHandler* leftDataHandler = new DataHandler();
		leftDataHandler->DataType = dataHandler->DataType;
		leftDataHandler->NumberOfDataPoints = leftDataPoints;
		leftDataHandler->DataPoints = new DataPoint*[leftDataPoints];

		DataHandler* rightDataHandler = new DataHandler();
		rightDataHandler->DataType = dataHandler->DataType;
		rightDataHandler->NumberOfDataPoints = rightDataPoints;
		rightDataHandler->DataPoints = new DataPoint*[rightDataPoints];

		rightDataPoints = 0;
		leftDataPoints = 0;
		for (int dataIndex = 0; dataIndex < dataHandler->NumberOfDataPoints; dataIndex++)
		{
			if (dataHandler->AskQuestion(node->Question, dataHandler->DataPoints[dataIndex]))
			{
				rightDataHandler->DataPoints[rightDataPoints] = dataHandler->DataPoints[dataIndex];
				rightDataPoints++;
			}
			else
			{
				leftDataHandler->DataPoints[leftDataPoints] = dataHandler->DataPoints[dataIndex];
				leftDataPoints++;
			}
		}

		delete dataHandler;
		// recurse down tree
		node->LeftChild = new DecisionTreeNode(node->Level + 1);
		GenerateTree(node->LeftChild, leftDataHandler, config);
		node->RightChild = new DecisionTreeNode(node->Level + 1);
		GenerateTree(node->RightChild, rightDataHandler, config);
	}
	else
	{
		node->NodeClass = MostLikelyNodeClass(dataHandler);
		delete dataHandler;
	}
}

Question* DecisionTree::ReturnBestQuestion(DataHandler* dataHandler, Config* config)
{
	int numberOfQuestionsToTry = dataHandler->ReturnNumberOfQuestions(dataHandler->DataType, config);
	Question** questionsToTry = dataHandler->ReturnQuestions(dataHandler->DataType, config);
	
	// initialize best question to the first question
	Question* bestQuestion = questionsToTry[0];
	double bestImpurity = ReturnQuestionImpurity(bestQuestion, dataHandler, config);
	// now try to improve best question
	for (int questionIndex = 0; questionIndex < numberOfQuestionsToTry; questionIndex++)
	{
		double impurity = ReturnQuestionImpurity(questionsToTry[questionIndex], dataHandler, config);
		if (bestImpurity > impurity)
		{
			bestQuestion = questionsToTry[questionIndex];
			bestImpurity = impurity;
		}
	}
	delete[] questionsToTry;
	return bestQuestion;
}

double DecisionTree::ReturnQuestionImpurity(Question* question, DataHandler* dataHandler, Config* config)
{
	//int* nodeClass0Size = new int[];
	int LeftClass0Size = 0;
	int LeftClass1Size = 0;
	int RightClass0Size = 0;
	int RightClass1Size = 0;
	double leftLog0 = 0;
	double leftLog1 = 0;
	double rightLog0 = 0;
	double rightLog1 = 0;
	for (int dataIndex = 0; dataIndex < dataHandler->NumberOfDataPoints; dataIndex++)
	{
		//DataPoint point = dataHandler->DataPoints[dataIndex];
		short pointClass = dataHandler->DataPoints[dataIndex]->ClassName;
		if (dataHandler->AskQuestion(question, dataHandler->DataPoints[dataIndex]))
		{
			if (pointClass == 0)
			{
				RightClass0Size++;
			}
			else
			{
				RightClass1Size++;
			}
		}
		else
		{
			if (pointClass == 0)
			{
				LeftClass0Size++;
			}
			else
			{
				LeftClass1Size++;
			}
		}
	}
	if (LeftClass0Size > 0)
	{
		leftLog0 = -log2((double)LeftClass0Size / (LeftClass0Size + LeftClass1Size));
	}
	if (LeftClass1Size > 0)
	{
		leftLog1 = -log2((double)LeftClass1Size / (LeftClass0Size + LeftClass1Size));
	}
	if (RightClass0Size > 0)
	{
		rightLog0 = -log2((double)RightClass0Size / (RightClass0Size + RightClass1Size));
	}
	if (RightClass1Size > 0)
	{
		rightLog1 = -log2((double)RightClass1Size / (RightClass0Size + RightClass1Size));
	}
	int totalLeft = LeftClass0Size + LeftClass1Size;
	int totalRight = RightClass0Size + RightClass1Size;
	return ((LeftClass0Size * leftLog0 + LeftClass1Size * leftLog1)
		+ (RightClass0Size * rightLog0 + RightClass1Size * rightLog1)) / (totalLeft + totalRight);
}

short DecisionTree::MostLikelyNodeClass(DataHandler* dataHandler)
{
	int class0Size = 0;
	int class1Size = 0;
	for (int dataIndex = 0; dataIndex < dataHandler->NumberOfDataPoints; dataIndex++)
	{
		if (dataHandler->DataPoints[dataIndex]->ClassName == 0)
		{
			class0Size++;
		}
		else
		{
			class1Size++;
		}
	}
	if (class1Size > class0Size)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

double DecisionTree::NodeEntropy(DataHandler* dataHandler)
{
	int class0Size = 0;
	int class1Size = 0;
	double class0log = 0;
	double class1log = 0;
	for (int dataIndex = 0; dataIndex < dataHandler->NumberOfDataPoints; dataIndex++)
	{
		if (dataHandler->DataPoints[dataIndex]->ClassName == 0)
		{
			class0Size++;
		}
		if (dataHandler->DataPoints[dataIndex]->ClassName == 1)
		{
			class1Size++;
		}
	}
	if (class0Size > 0)
	{
		class0log = -log2((double)class0Size / (class0Size + class1Size));
	}
	if (class1Size > 0)
	{
		class1log = -log2((double)class1Size / (class0Size + class1Size));
	}
	return (class0Size * class0log + class1Size * class1log) / (class0Size + class1Size);
}

short DecisionTree::ClassifyDataPoint(DataHandler* dataHandler, DataPoint* dataPoint, DecisionTreeNode* node)
{
	if (node->NodeClass != -1)
	{
		return node->NodeClass;
	}
	else
	{
		if (dataHandler->AskQuestion(node->Question, dataPoint))
		{
			return ClassifyDataPoint(dataHandler, dataPoint, node->RightChild);
		}
		else
		{
			return ClassifyDataPoint(dataHandler, dataPoint, node->LeftChild);
		}
	}
}