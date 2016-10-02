// BinaryDecisionForest.cpp : main project file.
// Customizable binary decision forest for categorical or continuous attribute values
// Two UCI data set examples included.

#include "stdafx.h"
#include "DecisionTree.h"
#include <string>
#include <fstream>

using namespace System;

DataHandler* ReturnCongressData(Config* config);
DataHandler* ReturnPimaData(Config* config);
void CategoricalAttributeExample();
void ContinuousAttributeExample();

void CategoricalAttributeExample()
{
	Console::WriteLine(L"Congressional voting records data set:");
	// analyze UCI Congreesional voting records data set: https://archive.ics.uci.edu/ml/datasets/Congressional+Voting+Records
	// set up parameters
	Config* config = new Config();
	config->MinEntropy = 0;
	config->MaxNumberOfLevels = 3;
	config->NumberOfTrees = 8;
	config->PercentageOfTreesToAgree = 0.5;
	config->NumberOfCategoricalAttributes = 16;
	config->MaxCategoryValues = new short[config->NumberOfCategoricalAttributes];
	for (int j = 0; j < config->NumberOfCategoricalAttributes; j++)
	{
		config->MaxCategoryValues[j] = 3;
	}
	config->NumberOfTrainingPoints = 200;
	config->NumberOfTestingPoints = 100;

	// set up data handler
	DataHandler* dataHandler = ReturnCongressData(config);

	// train forest
	DecisionTreeNode** forest = new DecisionTreeNode*[config->NumberOfTrees];
	for (int treeIndex = 0; treeIndex < config->NumberOfTrees; treeIndex++)
	{
		// pick subdivision of training points
		// each tree is trained on distinct sets of (numberOfTrainingPoints / numberOfTrees) data points
		DataHandler* trainingData = new DataHandler();
		trainingData->DataType = dataHandler->DataType;
		trainingData->NumberOfAttributes = dataHandler->NumberOfAttributes;
		trainingData->NumberOfAttributeValues = dataHandler->NumberOfAttributeValues;
		int numberOfDataPoints = config->NumberOfTrainingPoints / config->NumberOfTrees;
		int firstDataPoint = numberOfDataPoints * treeIndex;
		trainingData->NumberOfDataPoints = numberOfDataPoints;
		trainingData->DataPoints = new DataPoint*[numberOfDataPoints];
		for (int j = 0; j < numberOfDataPoints; j++)
		{
			trainingData->DataPoints[j] = dataHandler->DataPoints[firstDataPoint + j];
		}
		// train tree
		DecisionTree^ tree = gcnew DecisionTree(trainingData, config);
		forest[treeIndex] = tree->Root;
	}

	// set up testing data handler
	DataHandler* testingDataHandler = new DataHandler();
	testingDataHandler->NumberOfDataPoints = config->NumberOfTestingPoints;
	testingDataHandler->DataType = dataHandler->DataType;
	testingDataHandler->DataPoints = new DataPoint*[config->NumberOfTestingPoints];

	for (int j = 0; j < config->NumberOfTestingPoints; j++)
	{
		testingDataHandler->DataPoints[j] = dataHandler->DataPoints[config->NumberOfTrainingPoints + j];
	}

	// test data
	int falseDemCount = 0;
	int trueDemCount = 0;
	int falseRepCount = 0;
	int trueRepCount = 0;
	DecisionTree^ treeEngine = gcnew DecisionTree();
	int treesToAgree = config->PercentageOfTreesToAgree * config->NumberOfTrees;

	for (int j = 0; j < config->NumberOfTestingPoints; j++)
	{
		DataPoint* point = testingDataHandler->DataPoints[j];
		short treeClass = 0;
		for (int treeIndex = 0; treeIndex < config->NumberOfTrees; treeIndex++)
		{
			treeClass += treeEngine->ClassifyDataPoint(testingDataHandler, point, forest[treeIndex]);
		}

		if (treeClass >= treesToAgree && point->ClassName == 1) trueDemCount++;
		else if (treeClass >= treesToAgree && point->ClassName == 0) falseDemCount++;
		else if (treeClass < treesToAgree && point->ClassName == 1) falseRepCount++;
		else if (treeClass < treesToAgree && point->ClassName == 0) trueRepCount++;
	}

	double accuracy = (double)(trueDemCount + trueRepCount) / (trueDemCount + trueRepCount + falseDemCount + falseRepCount);
	Console::WriteLine("Accuracy: " + accuracy);
	Console::ReadLine();
}

void ContinuousAttributeExample()
{
	Console::WriteLine(L"Pima indians diabetes data set:");
	// analyze Pima Indian Diabetes data set: https://archive.ics.uci.edu/ml/datasets/Pima+Indians+Diabetes
	// set up parameters
	Config* config = new Config();
	config->MinEntropy = 0;
	config->MaxNumberOfLevels = 5;
	config->NumberOfTrees = 8;
	config->PercentageOfTreesToAgree = 0.5;
	config->NumberOfContinuousAttributes = 8;	
	config->NumberOfTrainingPoints = 400;
	config->NumberOfTestingPoints = 200;
	config->NumberOfRandomQuestions = 1000;

	// set up data handler & mine max/min attribute values
	DataHandler* dataHandler = ReturnPimaData(config);

	// train forest
	DecisionTreeNode** forest = new DecisionTreeNode*[config->NumberOfTrees];
	for (int treeIndex = 0; treeIndex < config->NumberOfTrees; treeIndex++)
	{
		// pick subdivision of training points
		// each tree is trained on distinct sets of (numberOfTrainingPoints / numberOfTrees) data points
		DataHandler* trainingData = new DataHandler();
		trainingData->DataType = dataHandler->DataType;
		trainingData->NumberOfAttributes = dataHandler->NumberOfAttributes;
		trainingData->NumberOfAttributeValues = dataHandler->NumberOfAttributeValues;
		int numberOfDataPoints = config->NumberOfTrainingPoints / config->NumberOfTrees;
		int firstDataPoint = numberOfDataPoints * treeIndex;
		trainingData->NumberOfDataPoints = numberOfDataPoints;
		trainingData->DataPoints = new DataPoint*[numberOfDataPoints];
		for (int j = 0; j < numberOfDataPoints; j++)
		{
			trainingData->DataPoints[j] = dataHandler->DataPoints[firstDataPoint + j];
		}
		// train tree
		DecisionTree^ tree = gcnew DecisionTree(trainingData, config);
		forest[treeIndex] = tree->Root;
	}

	// set up testing data handler
	DataHandler* testingDataHandler = new DataHandler();
	testingDataHandler->NumberOfDataPoints = config->NumberOfTestingPoints;
	testingDataHandler->DataType = dataHandler->DataType;
	testingDataHandler->DataPoints = new DataPoint*[config->NumberOfTestingPoints];

	for (int j = 0; j < config->NumberOfTestingPoints; j++)
	{
		testingDataHandler->DataPoints[j] = dataHandler->DataPoints[config->NumberOfTrainingPoints + j];
	}

	// test data
	int FP = 0;
	int TP = 0;
	int FN = 0;
	int TN = 0;
	DecisionTree^ treeEngine = gcnew DecisionTree();
	int treesToAgree = config->PercentageOfTreesToAgree * config->NumberOfTrees;

	for (int j = 0; j < config->NumberOfTestingPoints; j++)
	{
		DataPoint* point = testingDataHandler->DataPoints[j];
		short treeClass = 0;
		for (int treeIndex = 0; treeIndex < config->NumberOfTrees; treeIndex++)
		{
			treeClass += treeEngine->ClassifyDataPoint(testingDataHandler, point, forest[treeIndex]);
		}

		if (treeClass >= treesToAgree && point->ClassName == 1) TP++;
		else if (treeClass >= treesToAgree && point->ClassName == 0) FP++;
		else if (treeClass < treesToAgree && point->ClassName == 1) FN++;
		else if (treeClass < treesToAgree && point->ClassName == 0) TN++;
	}

	double accuracy = (double)(TP + TN) / (TP + TN + FP + FN);
	Console::WriteLine("Accuracy: " + accuracy);
	Console::ReadLine();
}

int main(array<System::String ^> ^args)
{
    Console::WriteLine(L"Basic Binary Decision Forest");
	// Learn UCI Congress Voting Records data set
	CategoricalAttributeExample();
	// Learn UCI Pima Indians Diabetes data set
	ContinuousAttributeExample();
    return 0;
}

DataHandler* ReturnCongressData(Config* config)
{
	std::string trainAddress = "C:\\Users\\TRussell\\Desktop\\ML Notes\\HW 1\\data\\Congress\\house-votes-84.txt";
	int numberOfDataPoints = 435;

	// set up data handler
	DataHandler* dataHandler = new DataHandler();
	dataHandler->NumberOfDataPoints = numberOfDataPoints;
	dataHandler->DataType = 1;
	dataHandler->DataPoints = new DataPoint*[dataHandler->NumberOfDataPoints];

	std::ifstream congressDataFile(trainAddress);
	for (int j = 0; j < numberOfDataPoints; j++)
	{
		dataHandler->DataPoints[j] = new DataPoint();
		dataHandler->DataPoints[j]->DataType = 1;
		std::string className;
		std::getline(congressDataFile, className, ',');
		if (className == "republican")
		{
			dataHandler->DataPoints[j]->ClassName = 0;
		}
		else
		{
			dataHandler->DataPoints[j]->ClassName = 1;
		}
		dataHandler->DataPoints[j]->CategoricalAttributes = new short[config->NumberOfCategoricalAttributes];
		for (int attributeIndex = 0; attributeIndex < config->NumberOfCategoricalAttributes - 1; attributeIndex++)
		{
			std::string attribute;
			std::getline(congressDataFile, attribute, ',');
			if (attribute == "y")
			{
				dataHandler->DataPoints[j]->CategoricalAttributes[attributeIndex] = 1;
			}
			if (attribute == "n")
			{
				dataHandler->DataPoints[j]->CategoricalAttributes[attributeIndex] = 2;
			}
			if (attribute == "?")
			{
				dataHandler->DataPoints[j]->CategoricalAttributes[attributeIndex] = 3;
			}
		}
		// final attribute
		std::string lastAttribute;
		std::getline(congressDataFile, lastAttribute, '\n');
		int index = config->NumberOfCategoricalAttributes - 1;
		if (lastAttribute == "y")
		{
			dataHandler->DataPoints[j]->CategoricalAttributes[index] = 1;
		}
		if (lastAttribute == "n")
		{
			dataHandler->DataPoints[j]->CategoricalAttributes[index] = 2;
		}
		if (lastAttribute == "?")
		{
			dataHandler->DataPoints[j]->CategoricalAttributes[index] = 3;
		}
	}
	// close input stream
	congressDataFile.close();

	return dataHandler;
}

DataHandler* ReturnPimaData(Config* config)
{
	std::string trainAddress = "C:\\Users\\TRussell\\Desktop\\ML Notes\\HW 1\\data\\Pima\\pima-indians-diabetes.data";
	int numberOfDataPoints = 768;

	// set up data handler
	DataHandler* dataHandler = new DataHandler();
	dataHandler->NumberOfDataPoints = numberOfDataPoints;
	dataHandler->DataType = 2;
	dataHandler->DataPoints = new DataPoint*[dataHandler->NumberOfDataPoints];

	std::ifstream pimaDataFile(trainAddress);
	for (int j = 0; j < numberOfDataPoints; j++)
	{
		dataHandler->DataPoints[j] = new DataPoint();
		dataHandler->DataPoints[j]->DataType = 2;
		dataHandler->DataPoints[j]->ContinuousAttributes = new double[config->NumberOfContinuousAttributes];
		for (int attributeIndex = 0; attributeIndex < config->NumberOfContinuousAttributes; attributeIndex++)
		{
			std::string attribute;
			std::getline(pimaDataFile, attribute, ',');
			dataHandler->DataPoints[j]->ContinuousAttributes[attributeIndex] = std::stod(attribute);
		}
		// class value
		std::string lastAttribute;
		std::getline(pimaDataFile, lastAttribute, '\n');
		int index = config->NumberOfContinuousAttributes;
		if (lastAttribute == "0")
		{
			dataHandler->DataPoints[j]->ClassName = 0;
		}
		else
		{
			dataHandler->DataPoints[j]->ClassName = 1;
		}
	}
	// close input stream
	pimaDataFile.close();

	//mine max and min values from training data
	config->MinContinuousValues = new double[config->NumberOfContinuousAttributes];
	config->MaxContinuousValues = new double[config->NumberOfContinuousAttributes];

	// initialize min and max values based on first data point
	for (int k = 0; k < config->NumberOfContinuousAttributes; k++)
	{
		config->MinContinuousValues[k] = dataHandler->DataPoints[0]->ContinuousAttributes[k];
		config->MaxContinuousValues[k] = dataHandler->DataPoints[0]->ContinuousAttributes[k];
	}

	// find max and min attribute values from training points
	for (int j = 1; j < config->NumberOfTrainingPoints; j++)
	{
		for (int k = 0; k < config->NumberOfContinuousAttributes; k++)
		{
			double value = dataHandler->DataPoints[j]->ContinuousAttributes[k];
			if (value < config->MinContinuousValues[k])
			{
				config->MinContinuousValues[k] = value;
			}
			if (value > config->MaxContinuousValues[k])
			{
				config->MaxContinuousValues[k] = value;
			}
		}
		
	}

	return dataHandler;
}
