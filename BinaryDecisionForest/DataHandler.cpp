#include "stdafx.h"
#include "DataHandler.h"
#include <time.h>
#include <stdlib.h>
#include <random>

DataHandler::DataHandler()
{
}

DataHandler::~DataHandler()
{
	delete[] DataPoints;
}

bool DataHandler::AskQuestion(Question* question, DataPoint* dataPoint)
{
	if (question->QuestionType == 1)
	{
		return dataPoint->CategoricalAttributes[question->Attribute] == question->LowerThreshold;
	}
	else
	{
		double value = dataPoint->ContinuousAttributes[question->Attribute];
		return question->LowerThreshold <= value && value <= question->UpperThreshold;
	}
}

int DataHandler::ReturnNumberOfQuestions(short dataType, Config* config)
{
	int count = 0;
	if (dataType == 1)
	{
		for (int j = 0; j < config->NumberOfCategoricalAttributes; j++)
		{
			count += config->MaxCategoryValues[j];
		}
	}
	else
	{
		count = config->NumberOfRandomQuestions;
	}
	return count;
}

Question** DataHandler::ReturnAllQuestions(short dataType, Config* config)
{
	Question** questionsToReturn = new Question*[ReturnNumberOfQuestions(dataType, config)];
	for (int j = 0, i=0; j < config->NumberOfCategoricalAttributes; j++)
	{
		for (int k = 1; k <= config->MaxCategoryValues[j]; k++, i++)
		{
			questionsToReturn[i] = new Question();
			questionsToReturn[i]->Attribute = j;
			questionsToReturn[i]->LowerThreshold = k;
			questionsToReturn[i]->QuestionType = dataType;
		}
	}
	return questionsToReturn;
}

Question** DataHandler::ReturnRandomQuestions(short dataType, Config* config)
{
	Question** questionsToReturn = new Question*[config->NumberOfRandomQuestions];
	srand(time(NULL));
	std::default_random_engine generator;
	for (int j = 0; j < config->NumberOfRandomQuestions; j++)
	{
		int attribute;
		double lowerThreshold;
		double upperThreshold;

		attribute = rand() % config->NumberOfContinuousAttributes;
		std::uniform_real_distribution<double> lDistribution(config->MinContinuousValues[attribute], config->MaxContinuousValues[attribute]);
		lowerThreshold = lDistribution(generator);
		std::uniform_real_distribution<double> rDistribution(lowerThreshold, config->MaxContinuousValues[attribute]);
		upperThreshold = rDistribution(generator);

		questionsToReturn[j] = new Question();
		questionsToReturn[j]->Attribute = attribute;
		questionsToReturn[j]->QuestionType = dataType;
		questionsToReturn[j]->LowerThreshold = lowerThreshold;
		questionsToReturn[j]->UpperThreshold = upperThreshold;
	}
	return questionsToReturn;
}

Question** DataHandler::ReturnQuestions(short dataType, Config* config)
{
	if (dataType == 1)
	{
		return ReturnAllQuestions(dataType, config);
	}
	else
	{
		return ReturnRandomQuestions(dataType, config);
	}
}
