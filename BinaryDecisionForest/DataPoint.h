#pragma once
# include "Config.h"

struct DataPoint
{
	DataPoint()
	{
		// constructor
	}

	~DataPoint()
	{
		// destructor
		if (CategoricalAttributes) delete[] CategoricalAttributes;
		if (ContinuousAttributes) delete[] ContinuousAttributes;
	}

	// Use data type to keep track of different types of data points
	// Default: 1 == categorical attributes, 2 == continuous attributes, 3 == both
	short DataType;
	// Ground truth class value
	short ClassName;
	// Store values of categorical attributes as shorts (small number of categories)
	short* CategoricalAttributes;
	// Store values of continuous attributes as doubles
	double* ContinuousAttributes;
};
