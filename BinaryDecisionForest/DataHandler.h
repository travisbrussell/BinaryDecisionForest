#pragma once
#include "Question.h"
#include "DataPoint.h"
#include "Config.h"

class DataHandler
{
public:
	DataHandler();
	~DataHandler();
	Question** ReturnRandomQuestions(short dataType, Config* config);
	bool AskQuestion(Question* question, DataPoint* dataPoint);
	int ReturnNumberOfQuestions(short dataType, Config* config);
	Question** ReturnAllQuestions(short dataType, Config* config);
	Question** ReturnQuestions(short dataType, Config* config);

	int NumberOfDataPoints;
	short DataType;
	short NumberOfAttributes;
	short* NumberOfAttributeValues;
	DataPoint** DataPoints;
};

