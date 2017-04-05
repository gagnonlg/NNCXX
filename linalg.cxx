#include "linalg.h"
#include "rng.h"


Matrix Matrix::uniform(size_t nrow, size_t ncol, float low, float high)
{
	Matrix matrix(nrow, ncol);
	for (size_t i = 0; i < nrow; i++)
		for (size_t j = 0; j < ncol; j++)
			matrix.set(i, j, Random::uniform(low, high));
	return matrix;
}
				
