#include "stdafx.h"
#include "BaseCecWriter.h"
#include "MathUtils.h"
#include <list>



BaseCecWriter::BaseCecWriter()
{
}


BaseCecWriter::~BaseCecWriter()
{
}

void BaseCecWriter::OpenFileForWrite()
{
}

void BaseCecWriter::WriteCecResults(char* pathToRunResults)
{
	char str[80];
	char cecFilePath[50];
	char *headerCec = "Func,Best,Worst,Median,Mean,Std\n";

	sprintf(cecFilePath, "%s/ShadeCEC.csv", pathToRunResults);
	_oFile = fopen(cecFilePath, "w");
	fprintf(_oFile, headerCec);

	for (int i = 1; i < 16; i++)
	{
		vector<double> *recordedErrorValues = new vector<double>();

		char filename[50];
		sprintf(filename, "%s/func%d.csv", pathToRunResults,i);
		_inFile = fopen(filename, "r+");

		double errorValue;

		for (int j = 0; j < _runs; j++)
		{
			for (int k = 0; k < 17; k++)
			{
				fscanf(_inFile, "%lf,", &errorValue);
				recordedErrorValues->push_back(errorValue);
			}
			fscanf(_inFile, "%c,", str);
			fscanf(_inFile, "%d\n", str);
		}

		fclose(_inFile);
		
		sort(recordedErrorValues->begin(), recordedErrorValues->end());

		double best = (*recordedErrorValues)[0];
		double worst = (*recordedErrorValues)[recordedErrorValues->size() - 1];
		double median = MathUtils::Median(recordedErrorValues);
		double mean = MathUtils::Mean(recordedErrorValues);
		double stdDev = MathUtils::StdDev(recordedErrorValues, median);

		fprintf(_oFile, "%d,%f,%f,%f,%f,%f\n", i, best, worst, median, mean, stdDev);

		delete recordedErrorValues;
	}

	fclose(_oFile);
}