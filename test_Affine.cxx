#include <iostream>

#include "network.h"

int main(void)
{
	AffineMap am(10, 5);
	gsl_matrix_float *X = gsl_matrix_float_calloc(20, 10);
	gsl_matrix_float *Y = am.forward_propagate(X);

	for (size_t i = 0; i < Y->size1; i++) {
		for (size_t j = 0; j < Y->size2; j++)
			std::cout << gsl_matrix_float_get(Y, i, j) << ' ';
		std::cout << '\n';
	}
}
