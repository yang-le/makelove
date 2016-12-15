#include <stdio.h>
#include <math.h>

typedef float (*func1_t)(float x);

static float test_func0(float x)
{
	//return exp(-pow(x, 2) / 2) / sqrt(2 * M_PI);
	return 1;
}

static float test_func1(float x)
{
	return pow(x, 3) + 1.1 * pow(x, 2) + 0.9 * x - 1.4;
}

static float test_func2(float x)
{
	return exp(-pow(x, 2));
}

static float test_func3(float x)
{
	return sqrt(2) * sin(x);
}

float resolve1(func1_t func, float a, float b, float delta)
{
	float value = func((a + b) / 2);

	while (fabs(value) > delta) {		
		if ((value > 0) == (func(a) > 0)) {
			a = (a + b) / 2;
		} else {
			b = (a + b) / 2;
		}
		value = func((a + b) / 2);
	}

	return (a + b) / 2;
}

float integral1(func1_t func, float a, float b, unsigned int n)
{
	float t0 = func(a) + func(b);
	
	float t1 = 0;
	for (unsigned int i = 2; i <= n - 2; i += 2) {
		t1 += func(a + i * (b - a) / n);
	}
	t1 *= 2;

	float t2 = 0;
	for (unsigned int i = 1; i <= n - 1; i += 2) {
		t2 += func(a + i * (b - a) / n);
	}
	t2 *= 4;
	
	return (b - a) * (t0 + t1 + t2) / (3 * n);
}

float mean1(func1_t func, float a, float b, unsigned int n)
{
	return integral1(func, a, b, n) / (b - a);
}

float rms1(func1_t func, float a, float b, unsigned int n)
{
	float t0 = pow(func(a), 2) + pow(func(b), 2);
	
	float t1 = 0;
	for (unsigned int i = 2; i <= n - 2; i += 2) {
		t1 += pow(func(a + i * (b - a) / n), 2);
	}
	t1 *= 2;

	float t2 = 0;
	for (unsigned int i = 1; i <= n - 1; i += 2) {
		t2 += pow(func(a + i * (b - a) / n), 2);
	}
	t2 *= 4;
	
	return sqrt((t0 + t1 + t2) / (3 * n));
}

float mse1(func1_t func, float a, float b, unsigned int n)
{
	float mean = mean1(func, a, b, n);

	float t0 = pow(func(a) - mean, 2) + pow(func(b) - mean, 2);
	
	float t1 = 0;
	for (unsigned int i = 2; i <= n - 2; i += 2) {
		t1 += pow(func(a + i * (b - a) / n) - mean, 2);
	}
	t1 *= 2;

	float t2 = 0;
	for (unsigned int i = 1; i <= n - 1; i += 2) {
		t2 += pow(func(a + i * (b - a) / n) - mean, 2);
	}
	t2 *= 4;
	
	return sqrt((t0 + t1 + t2) / (3 * n));
}

int main(int argc, char* argv[])
{
	float result = resolve1(test_func1, 0, 1, 0.0001);
	printf("get result %f at x = %f\n", test_func1(result), result);

	result = integral1(test_func2, 0, 1, 1000);
	printf("get result %f\n", result);

	result = rms1(test_func3, 0, 2 * M_PI, 1000);
	printf("get result %f\n", result);

	result = mean1(test_func0, -1, 1, 1000);
	printf("get result %f\n", result);

	result = mse1(test_func0, -1, 1, 1000);
	printf("get result %f\n", result);
}

