#pragma once
#include <algorithm>
using namespace std;
class BaseCecWriter
{
public:
	BaseCecWriter();
	virtual ~BaseCecWriter();
	virtual void OpenFileForWrite();
	void WriteCecResults(char * pathToRunResults);
	FILE * _oFile;
	FILE * _inFile;
	int _dim;
	int _runs;
};

