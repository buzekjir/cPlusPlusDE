#include "stdafx.h"
#include "Shade.h"

using namespace std;


Shade::Shade()
{
	rand_device = new random_device();
	rand = new mt19937((*rand_device)());
};

void Shade::initAndRun(Cec15 *instance, double *lowerBound, double *upperBound, int N, int dim, int nde, int H, int func_num)
{
	uddist = new uniform_real_distribution<double>(0.0, 1.0);
	uidist = new uniform_int_distribution<int>(0, N-1);
	normddist = new normal_distribution<double>(norm_mean, norm_stddev);
	cddist = new cauchy_distribution<double>(location, scale);

	double *uBlBDiff = new double[dim];

	for (int i = 0; i < dim; i++)
	{
		uBlBDiff[i] = upperBound[i] - lowerBound[i];
	}

	int mplLen = dim*N;
	double *mainpopLinear = new double[mplLen];
	double *fevals = new double[N];

	double* ptrDpop = mainpopLinear;
	double* ptrDdiff = uBlBDiff;
	double* ptrLb = uBlBDiff;

	for (ptrDpop = mainpopLinear; ptrDpop < mainpopLinear + mplLen;)
	{
		for (ptrDdiff = uBlBDiff,ptrLb = lowerBound; ptrDdiff < uBlBDiff + dim; ptrDdiff++, ptrLb++)
		{
			*ptrDpop = (*uddist)(*rand) * (*ptrDdiff) + (*ptrLb);
			ptrDpop++;
		}
	}

	instance->cec15_test_func(mainpopLinear, fevals, dim, N, func_num);

	double f_min = DBL_MAX;

	for (int i = 0; i < N; i++)
	{
		if (fevals[i] < f_min)
		{
			f_min = fevals[i];
		}
	}

	run(instance, lowerBound, upperBound, N, dim, nde, H, mainpopLinear, fevals, func_num, &f_min);

	delete uidist;
	delete normddist;
	delete cddist;
	delete uddist;
	delete []uBlBDiff;
	delete []mainpopLinear;
	delete []fevals;
};

void Shade::run(Cec15 *instance, double *lowerBound, double *upperBound, int N, int dim, int nde, int H, double *mainPopulation, double *fevals, int func_num, double *f_min)
{
	double *tempPopulation = new double[dim*N];
	double *tempFevals = new double[N];
	double *m_cr = new double[H];
	double *m_f = new double[H];
	double *A = new double[dim*N];
	double *fDiff = new double[N];

	double *S_f = new double[N];
	double *S_cr = new double[N];

	double **evalsIndicies = new double*[N];

	double** mainPopPtrs = new double*[N];
	double** tempPopPtrs = new double*[N];
	double** Aptrs = new double*[N];
	double** Q = nullptr;
	double* Qt = nullptr;

	double cr_i, f_i;

	int sCount, aCount = 0;
	int k = 0;
	int r_i;
	double p_min = 2 / double(N);
	double p_i;

	double *rp_best = nullptr, *r_1 = nullptr, *r_2 = nullptr;

	double *xPtr, *yPtr, *zPtr, *current;

	for (xPtr = m_cr; xPtr < m_cr + H; xPtr++)
	{
		*xPtr = 0.5;
	}

	for (xPtr = m_f; xPtr < m_f + H; xPtr++)
	{
		*xPtr = 0.5;
	}
	int i = 0, j;
	int j_rand;

	//TODO: spatial locality?
	for (i = 0; i < N; i++)
	{
		mainPopPtrs[i] = &mainPopulation[i*dim];
		tempPopPtrs[i] = &tempPopulation[i*dim];
		Aptrs[i] = &A[i*dim];
	}	

	while (nde > 0)
	{
		sCount = 0;

		for (xPtr = tempFevals , yPtr = fevals, i = 0; xPtr < tempFevals + N; xPtr++ , yPtr++ , i++)
		{
			*xPtr = *yPtr; // fevals -> tempFevals
			evalsIndicies[i] = mainPopPtrs[i]; // indicies -> evalsIndicies
		}

		quicksort(tempFevals, evalsIndicies, 0, N);

		for (i = 0; i < N; i++)
		{
			r_i = getNextInt(0, H);

			cr_i = nRand(m_cr[r_i], norm_stddev);
			f_i = cRand(m_f[r_i], scale);

			if (cr_i < 0)cr_i = 0;
			if (cr_i > 1)cr_i = 1;
			while (f_i <= 0) f_i = cRand(m_f[r_i], scale);
			if (f_i > 1)f_i = 1;

			S_cr[i] = cr_i;
			S_f[i] = f_i;

			p_i = p_min + (getNext() * (0.2 - p_min));
			
			current = mainPopPtrs[i];
			rp_best = evalsIndicies[getNextInt(0, int(round(p_i * N)))];
			r_1 = mainPopPtrs[(*uidist)(*rand)];

			j = getNextInt(0, N + sCount);

			if (j > N - 1)
			{
				r_2 = Aptrs[j-N];
			}
			else
			{
				r_2 = mainPopPtrs[j];
			}

			while (current == r_1) r_1 = mainPopPtrs[(*uidist)(*rand)];
			while (current == r_2 || r_1 == r_2)
			{
				j = getNextInt(0, N + sCount);
				if (j > N - 1)r_2 = Aptrs[j - N];
				else r_2 = mainPopPtrs[j];
			}

			j_rand = getNextInt(0,dim);

			zPtr = tempPopPtrs[i];
			for (xPtr = zPtr, j = 0; xPtr < zPtr + dim; xPtr++, j++)
			{
				if (j_rand == j || (*uddist)(*rand) < cr_i)
				{
					*xPtr = *current + f_i * (*rp_best - *current) + f_i * (*r_1 - *r_2);
					
				}else
				{
					*xPtr = *current;
				}

				current++;
				rp_best++;
				r_1++;
				r_2++;
			}
		}//endfor

		instance -> cec15_test_func(tempPopulation, tempFevals, dim, N, func_num);

		nde -= N;

		//selekce
		for (xPtr = tempFevals, yPtr = fevals, i = 0; i < N; i++, xPtr++, yPtr++)
		{
			if (*xPtr <= *yPtr)//sucesfull
			{
				if (*xPtr < *yPtr)
				{
					if (aCount == N)
					{
						double *aDest = Aptrs[getNextInt(0, N)];
						memcpy(aDest, mainPopPtrs[i], sizeof(double)*dim);
					}
					else
					{
						double *aDest = Aptrs[aCount];
						memcpy(aDest, mainPopPtrs[i], sizeof(double)*dim);
						aCount++;
					}

					S_cr[sCount] = S_cr[i];
					S_f[sCount] = S_f[i];
					fDiff[sCount] = abs(*xPtr - *yPtr);
					sCount++;
				}
				
				*yPtr = *xPtr;

				if (*yPtr < *f_min)
				{
					*f_min = *yPtr;
				}

				rec->IncSuccMember();
			}
			else
			{
				memcpy(tempPopPtrs[i], mainPopPtrs[i], sizeof(double)*dim);
			}
		}
		
		rec->TryRecordErrorValue(*f_min - 100 * func_num, nde);
		
		Q = mainPopPtrs;
		mainPopPtrs = tempPopPtrs;
		tempPopPtrs = Q;

		Qt = mainPopulation;
		mainPopulation = tempPopulation;
		tempPopulation = Qt;

		if (sCount != 0)
		{
			double meanWA = 0.0;

			double fKSum = 0.0;
			for (i = 0; i < sCount; i++) fKSum += fDiff[i];

			for (i = 0; i < sCount; i++) meanWA += (fDiff[i] / fKSum) * S_cr[i];

			double meanWL;
			double sfUsum = 0.0;
			double sfLsum = 0.0;

			for (i = 0; i < sCount; i++)sfUsum += (fDiff[i] / fKSum) * (S_f[i] * S_f[i]);
			for (i = 0; i < sCount; i++)sfLsum += (fDiff[i] / fKSum) * S_f[i];

			meanWL = sfUsum / sfLsum;
			m_cr[k] = meanWA;
			m_f[k] = meanWL;

			k++;

			if (k == H)
			{
				k = 0;
			}
		}
	}

	delete[] tempPopulation;
	delete[] tempFevals;
	delete[] m_cr;
	delete[] m_f;
	delete[] A;
	delete[] fDiff;

	delete[] S_f;
	delete[] S_cr;

	for (i = 0; i < N; i++)
	{
		evalsIndicies[i] = nullptr;
		mainPopPtrs[i] = nullptr;
		tempPopPtrs[i] = nullptr;
		Aptrs[i] = nullptr;
	}

	delete[] evalsIndicies;
	delete[] mainPopPtrs;
	delete[] tempPopPtrs;
	delete[] Aptrs;
}

void Shade::quicksort(double *array, double **indicies, int left, int right)
{
	if (left < right)
	{
		int boundary = left;
		for (int i = left + 1; i < right; i++)
		{
			if (array[i] < array[left])
			{
				swap(array, indicies, i, ++boundary);
			}
		}
		swap(array, indicies, left, boundary);
		quicksort(array, indicies, left, boundary);
		quicksort(array, indicies, boundary + 1, right);
	}
}

void Shade::swap(double *array, double **indicies, int left, int right)
{
	double tmpD = array[right];
	array[right] = array[left];
	array[left] = tmpD;

	double *tempI = indicies[right];
	indicies[right] = indicies[left];
	indicies[left] = tempI;
}

inline int Shade::getNextInt(int min, int max)
{
	decltype((*uidist).param()) new_param(min, max-1);
	return (*uidist)(*rand, new_param);
}

inline double Shade::getNext(double min, double max)
{
	decltype((*uddist).param()) new_param(min, max);
	return (*uddist)(*rand, new_param);
}

inline double Shade::getNext()
{
	return (*uddist)(*rand);
}

inline double Shade::nRand(double mean, double std_dev)
{
	decltype((*normddist).param()) cr_i_param(mean, std_dev);
	return (*normddist)(*rand, cr_i_param);
}

inline double Shade::cRand(double loc, double scl)
{
	decltype((*cddist).param()) f_i_param(loc, scl);
	return (*cddist)(*rand, f_i_param);
}

Shade::~Shade()
{
	delete rand_device;
	delete rand;
	//delete uidist;
	//delete uidist;
	//delete rand;
	/*;
	*/
};

