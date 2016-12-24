#include <stdio.h>
#include <math.h>

/* note: use -O2 to speed up */

static double hypothesis(int n, double *x, double *theta)
{
	int i = 0;
	double sum = theta[n];

	for (i = 0; i < n; ++i) {
		sum += x[i] * theta[i];
	}

	return sum;
}

void bgd(
	double alpha, /* the learning rate */
	int m, /* the size of training set*/
	int n, /* the size of feature (dimension of x) */
	double *y, /* the targets */
	double *x, /* the features */
	double *theta /* the parameter */
) {
	double allSum = 0;

	do {
		int i = 0;
		int j = 0;

		double sum = 0;

		for (i = 0; i < m; ++i) {
			sum += hypothesis(n, &(x[i * n]), theta) - y[i];
		}
		
		theta[n] -= alpha * sum;
		allSum = fabs(sum);

		for (j = 0; j < n; ++j) {
			sum = 0;
			for (i = 0; i < m; ++i) {
				sum += (hypothesis(n, &(x[i * n]), theta) - y[i]) * x[i * n + j];
			}
			theta[j] -= alpha * sum;
			allSum += fabs(sum);
		}
#ifndef NDEBUG
		printf("allSum %f\n", allSum);
#endif
	} while (allSum > 0.1);
}

#if 1
static double prices[] = {400, 330, 369, 232, 540};
static double features[] = {2104, 3, 1600, 3, 2400, 3, 1416, 2, 3000, 4};
// 0.063969 103.222058 -70.043756
static double params[3] = {0};

int main (int argc, char **argv)
{
	bgd(0.00000008, 5, 2, prices, features, params);
	printf("params = %f %f %f\n", params[0], params[1], params[2]);
	return 0;
}
#endif

