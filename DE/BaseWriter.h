#pragma once
#include "stdafx.h"
#include <deque>
#include <list>
using namespace std;

class BaseWriter
{
private:
public:
	BaseWriter(int max_evals);
	virtual ~BaseWriter();
	virtual void OpenFileForWrite(int i);
	void CloseFile();
	void RenewFesTresholds();
	void TryRecordErrorValue(double fmin, int nde);
	void IncSuccMember();
	void WriteResults();
	FILE * _onFile;
	deque<double> *fesTresholds;
	list<double> *errorValues;
	int _max_evals;
	int _num_succeses;
};

