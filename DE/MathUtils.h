#pragma once
class MathUtils
{
public:
	MathUtils();
	virtual ~MathUtils();
	static double Median(std::vector<double> *input);
	static double Mean(std::vector<double> *input);
	static double StdDev(std::vector<double> *input, double median);
};

