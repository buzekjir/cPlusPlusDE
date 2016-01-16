#pragma once
#include "BaseWriter.h"
class ShadeWriter :	public BaseWriter
{
public:
	ShadeWriter(int max_evals) : BaseWriter(max_evals){};
	virtual ~ShadeWriter();
	void OpenFileForWrite(int i) override;
};

