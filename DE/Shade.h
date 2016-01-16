#pragma once
#include "stdafx.h"
#include "Cec15.h"
#include "BaseWriter.h"

using namespace std;


#define norm_stddev 0.31622776601 //TODO: less digits?
#define norm_mean 0.5
#define location 0.5
#define scale 0.1

class Shade
{
private:
	random_device * rand_device;
	mt19937 *rand;
	uniform_real_distribution<double> *uddist;
	uniform_int_distribution<int> *uidist;
	normal_distribution<double> *normddist;
	cauchy_distribution<double> *cddist;
	void run(Cec15 *instance, double *, double *, int N, int dim, int nde, int H, double *mainPopulation, double *fevals, int func_num, double *f_min);
	void quicksort(double *, double **, int left, int right);
	void swap(double *, double **, int left, int right);
	int getNextInt(int min, int max);
	double getNext(double min, double max);
	double getNext();
	double nRand(double mean, double std_dev);
	double cRand(double loc, double scl);
public:
	Shade();
	~Shade();
	void initAndRun(Cec15 *instance, double *, double *, int N, int dim, int nde, int H, int func_num);
	BaseWriter *rec;
};