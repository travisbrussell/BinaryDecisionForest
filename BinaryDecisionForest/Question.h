#pragma once

struct Question
{
	Question()
	{
		// constructor
	}

	~Question()
	{
		// destructor
	}

	int Attribute;
	short QuestionType; // 1 == Categorical question; 2 == double random question;
	double LowerThreshold; // Lower threshold for random question generation; Category value for categorical attribute
	double UpperThreshold; // Upper threshold for random question generation; Ignore for categorical attribute
};