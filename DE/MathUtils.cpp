#include "stdafx.h"
#include "MathUtils.h"


MathUtils::MathUtils()
{
}


MathUtils::~MathUtils()
{
}

double MathUtils::Median(std::vector<double> *input)
{
	//assume input is already sorted.
	double median = 0.0;
	int size = input->size();

	if (size & 1)
	{
		median = (*input)[size / 2];
	}
	else
	{
		median = ((*input)[size / 2 - 1] + (*input)[size / 2]) / 2;
	}

	return median;
}

double MathUtils::Mean(std::vector<double>* input)
{
	double sum = 0.0;
	int size = input->size();

	for (int i = 0; i < size; i++)
	{
		sum += (*input)[i];
	}

	return sum / size;
}

double MathUtils::StdDev(std::vector<double>* input, double median)
{
	double delta = 0.0;
	int size = input->size();

	for (int i = 0; i < size; i++)
	{
		delta += ((*input)[i] - median) * ((*input)[i] - median);
	}

	delta = delta / size;

	return sqrt(delta);
}
