#include "stdafx.h"
#include "BaseWriter.h"
#include <array>


BaseWriter::BaseWriter(int max_evals)
{
	errorValues = new list<double>();
	RenewFesTresholds();
	_max_evals = max_evals;
}


BaseWriter::~BaseWriter()
{
	delete errorValues;
	delete fesTresholds;
}

void BaseWriter::OpenFileForWrite(int i)
{
}

void BaseWriter::CloseFile()
{
	fclose(_onFile);
}

void BaseWriter::RenewFesTresholds()
{
	array<double, 17> fesTresholdsArray = { 0.0001, 0.001, 0.01, 0.02, 0.03, 0.04, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 };

	fesTresholds = new deque<double>(fesTresholdsArray.begin(), fesTresholdsArray.end());
}

void BaseWriter::TryRecordErrorValue(double fmin, int nde)
{
	if (_max_evals - nde >= fesTresholds->front() * _max_evals)
	{
		errorValues->push_back(fmin);
		fesTresholds->pop_front();
	}
}

void BaseWriter::IncSuccMember()
{
	_num_succeses++;
}

void BaseWriter::WriteResults()
{
	for (list<double>::iterator i = errorValues->begin(); i != errorValues->end(); ++i)
	{
		fprintf(_onFile, "%f,", *i);
	}

	fprintf(_onFile, "%c,", ' ');
	fprintf(_onFile, "%d\n", _num_succeses);

}