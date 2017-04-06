#include <stdexcept>
#include <string>

#include <gsl/gsl_blas.h>

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

Matrix Matrix::matmul(Matrix& mat)
{
	auto right_size = __size;
	auto left_size = mat.size();

	if (right_size.second != left_size.first) {
		throw std::domain_error(
			"Second dimension of first matrix "
			"does not match first dimension of second matrix!"
			);
	}
	       	
	Matrix result = Matrix(__size.first, mat.size().second);

	gsl_blas_sgemm(CblasNoTrans, CblasNoTrans,
		       1.0, __matrix, mat.__matrix, 0.0, result.__matrix);

	return result;
}

