#pragma once

struct Config
{
	Config()
	{
		// constructor
	}

	~Config()
	{
		// destructor
	}

	// parameters for training //
	double MinEntropy; // If entropy is below this value, create a leaf node
	int MaxNumberOfLevels; // If we reach this level, create leaf node
	int NumberOfTrees;
	double PercentageOfTreesToAgree;
	int NumberOfRandomQuestions; // If questions are random, set number of questions to try
	int NumberOfCategoricalAttributes;
	short* MaxCategoryValues; // Set values from 1 to MaxValues
	int NumberOfContinuousAttributes;
	double* MaxContinuousValues; // Max value to consider for random question generation
	double* MinContinuousValues; // Min value to consider for random question generation
	int NumberOfTrainingPoints;
	int NumberOfTestingPoints;
};
