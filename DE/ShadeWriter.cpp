#include "stdafx.h"
#include "ShadeWriter.h"
#include <stdio.h>

ShadeWriter::~ShadeWriter()
{
}

void ShadeWriter::OpenFileForWrite(int i)
{
	char filename[50];
	sprintf(filename, "records/shade/func%d.csv", i);
	_onFile = fopen(filename, "a");
}