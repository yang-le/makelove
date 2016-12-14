#include <stdio.h>
#include "mnist.h"

int main(int argc, char* argv[])
{
	idx_t *mnist_data = mnist_open(argv[1]);
	printf("type = %d, dims = %d\n", mnist_data->type, mnist_data->dims);
	for (int i = 0; i < mnist_data->dims; ++i) {
		printf("dim[%d] = %d\n", i, mnist_data->dim[i]);
	}
	mnist_close(mnist_data);
	return 0;
}

