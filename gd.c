#include <stdio.h>
#include <memory.h>
#include <math.h>

/* note: use -O2 to speed up */

#define VERY_SMALL_NUM 0.01

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
	double sum = 0;

	do {
		int i = 0;
		int j = 0;

		double temp = 0;

		for (i = 0; i < m; ++i) {
			temp += hypothesis(n, &(x[i * n]), theta) - y[i];
		}
		
		theta[n] -= alpha * temp;
		sum = fabs(temp);

		for (j = 0; j < n; ++j) {
			temp = 0;
			for (i = 0; i < m; ++i) {
				temp += (hypothesis(n, &(x[i * n]), theta) - y[i]) * x[i * n + j];
			}
			theta[j] -= alpha * temp;
			sum += fabs(temp);
		}
#ifndef NDEBUG
		printf("sum %f\n", sum);
#endif
	} while (sum > VERY_SMALL_NUM);
}

void sgd(
	double alpha, /* the learning rate */
	int m, /* the size of training set*/
	int n, /* the size of feature (dimension of x) */
	double *y, /* the targets */
	double *x, /* the features */
	double *theta /* the parameter */
) {
	double sum = 0;

	do {
		int i = 0;

		for (i = 0; i < m; ++i) {
			int j = 0;
			double temp = hypothesis(n, &(x[i * n]), theta) - y[i];

			theta[n] -= alpha * temp;
			sum = fabs(temp);

			for (j = 0; j < n; ++j) {
				temp = (hypothesis(n, &(x[i * n]), theta) - y[i]) * x[i * n + j];
				theta[j] -= alpha * temp;
				sum += fabs(temp);
			}

			if (sum <= VERY_SMALL_NUM) {
				break;
			}
		}
#ifndef NDEBUG
		printf("sum %f\n", sum);
#endif
	} while (sum > VERY_SMALL_NUM);
}

#if 1
static double prices[] = {1000, 3000, 5000, 7000, 9000};
static double features[] = {0, 1, 2, 3, 4};
static double params[2] = {0};

int main (int argc, char **argv)
{
	bgd(0.01, 5, 1, prices, features, params);
	printf("bgd params = %f %f\n", params[0], params[1]);

	memset(params, 0, sizeof(params));

	sgd(0.01, 5, 1, prices, features, params);
	printf("sgd params = %f %f\n", params[0], params[1]);
	return 0;
}
#endif

