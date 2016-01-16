// DE.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Cec15.h"
#include "Shade.h"
#include "BaseWriter.h"
#include "ShadeWriter.h"
#include "DirHandler.h"
#include "BaseCecWriter.h"

int _tmain()
{
	//// obtain a seed from the system clock:
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	//std::mt19937 generator(seed);  // mt19937 is a standard mersenne_twister_engine
	//std::cout << "Random value: " << generator() << std::endl;

	//Cec15 *instance = new Cec15();

	//double *x = new double[30*50];// { -41.6941131692283, 68.7283406037282, -69.9282351411097, 74.4489388225257, -72.2608100084711, 30.7891834914185, -37.9969525459388, -46.5554589431154, 66.0530590913168, -33.7038003191847, -37.5070859706901, 0.784682707761359, 13.1385895325398, -75.209124960927, -69.9655913481275, 71.8697563123254, 21.5650090570933, 52.8358027528392, -21.7878112951066, 41.0504305028965, 18.1332573905819, 56.0348625754507, 63.0570853972064, 2.88334110678439, 42.926982417413, 46.8775124608242, 18.6669152837471, 31.7615283242217, -23.5342199906277, -68.7117255747132 };
	//double y[] = { 0.0 };
	//int n = 30; //dimenze
	//int m = 1; //pocet jedincu
	//int func_num = 1;
	//
	//instance->cec15_test_func(x, y, n, m, func_num);

	//cout << y[0];

	DirHandler::CreateDirectories();

	int num_runs = 51;
	printf("\n");

	for (int i = 1; i < 16; i++)
	{
		for (int j = 0; j < num_runs; j++)
		{
			int func_num = i;
			int nde = 300000;

			BaseWriter *writer = new ShadeWriter(nde);
			writer->OpenFileForWrite(func_num);

			Cec15 *instance = new Cec15();
			Shade * shadeInstance = new Shade();
			shadeInstance->rec = writer;

			double *upperBound = new double[30];
			double *lowerBound = new double[30];

			for (int k = 0; k < 30; k++)
			{
				upperBound[k] = 100;
				lowerBound[k] = -100;
			}

			shadeInstance->initAndRun(instance, lowerBound, upperBound, 50, 30, nde, 12, func_num);
			
			//console out
			printf("\rfunc%d %d of %d     ", func_num,j+1,num_runs);

			delete shadeInstance;
			delete instance;
			delete[] upperBound;
			delete[] lowerBound;

			writer->WriteResults();
			writer->CloseFile();
			delete writer;
		}
		printf("  completed..\n");
	}

	printf("writing CEC results\n");
	BaseCecWriter * writer = new BaseCecWriter();
	writer->_dim = 30;
	writer->_runs = 51;
	writer->WriteCecResults("records/shade");
	delete writer;
	printf("completed..\n");
	
	return 0;
}

