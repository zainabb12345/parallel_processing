#include <iostream>
#include <math.h>
#include <chrono>

/* ==================================================
   This file should / must not be changed !!!
   ================================================== */

/* Runtime of the functions in milliseconds. */
static const int runtime = 1000;

/* How often to repeat the computation for the desired runtime. */
static int repeat;

/* Perform a small benchmark to determine the value of 'repeat'. */
void initialize() 
{
	repeat = 0;
	volatile static double trash;
	double res = 0;
	std::chrono::duration<float> duration = std::chrono::milliseconds(runtime);
	auto start = std::chrono::high_resolution_clock::now();
	while (std::chrono::high_resolution_clock::now() - start < duration) {
		for (int i=0; i<100000; i++)
			res += sin((i+1000*repeat)/100.0);
		repeat++;
	}
	trash = res;
}

/*
** Auxiliary function: calculates for a given time
*/
static void doCompute()
{
	volatile static double trash;
	double res = 0;
	for (int n=0; n<repeat; n++) {
		for (int i=0; i<100000; i++)
			res += sin((i+1000*n)/100.0);
	}
	trash = res;
}

/*
** Compute intensive functions ...
*/
double f1()
{
	std::cout << "f1 started" << std::endl;
	doCompute();
	std::cout << "f1 ended" << std::endl;
	return 0.13456;
}

double f2(double a)
{
	std::cout << "f2 started" << std::endl;
	doCompute();
	std::cout << "f2 ended" << std::endl;
	return a*1.1;
}

double f3(double a)
{
	std::cout << "f3 started" << std::endl;
	doCompute();
	std::cout << "f3 ended" << std::endl;
	return a+7.65;
}

double f4(double a)
{
	std::cout << "f4 started" << std::endl;
	doCompute();
	std::cout << "f4 ended" << std::endl;
	return 999*a;
}

double f5(double a)
{
	std::cout << "f5 started" << std::endl;
	doCompute();
	std::cout << "f5 ended" << std::endl;
	return 1e20*a/0.148016;
}

double f6(double a, double b)
{
	std::cout << "f6 started" << std::endl;
	doCompute();
	std::cout << "f6 ended" << std::endl;
	return a-1.1+b;
}

double f7(double a)
{
	std::cout << "f7 started" << std::endl;
	doCompute();
	std::cout << "f7 ended" << std::endl;
	return -1e20+a*1.234e8;
}

