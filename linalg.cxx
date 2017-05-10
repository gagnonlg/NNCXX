#include <stdexcept>
#include <string>

#include <gsl/gsl_blas.h>

#include "linalg.h"
#include "rng.h"


void Matrix::set(Matrix &other, bool allow_resize)
{
	std::pair<size_t, size_t> o_size = other.size();
	if (o_size.first != __size.first || o_size.second != __size.second) {
		if (allow_resize) {
			gsl_matrix_float_free(__matrix);
			__matrix = gsl_matrix_float_calloc(o_size.first, o_size.second);
			__size = o_size;
		}
		else {
			throw std::domain_error(
				"Dimension of source matrix do not match those "
				"of destination and resize not allowed"
				);
		}
	}
	gsl_matrix_float_memcpy(__matrix, other.__matrix);
}

Matrix Matrix::uniform(size_t nrow, size_t ncol, float low, float high)
{
	Matrix matrix(nrow, ncol);
	for (size_t i = 0; i < nrow; i++)
		for (size_t j = 0; j < ncol; j++)
			matrix.set(i, j, Random::uniform(low, high));
	return matrix;
}


Matrix Matrix::matmul(Matrix& mat, bool trans_1, bool trans_2)
{
	auto right_size = mat.size();

	size_t r_row;
	size_t r_col;

	if (trans_1 && trans_2) {
		r_row = __size.second;
		r_col = right_size.first;
	} else if (trans_1 && !trans_2) {
		r_row = __size.second;
		r_col = right_size.second;
	} else if (!trans_1 && trans_2) {
		r_row = __size.first;
		r_col = right_size.first;
	} else { // if (!trans_1 && !trans_2)
		r_row = __size.first;
		r_col = right_size.second;
	}

	if ((!trans_1 && !trans_2 && __size.second != right_size.first) ||
	    (!trans_1 && trans_2 && __size.second != right_size.second) ||
	    (trans_1 && !trans_2 && __size.first != right_size.first) ||
	    (trans_1 && trans_2 && __size.first != right_size.second))
		throw std::domain_error(
			"Second dimension of first matrix "
			"does not match first dimension of second matrix!"
			);

	
	Matrix result = Matrix(r_row, r_col);

	gsl_blas_sgemm(
		(trans_1)? CblasTrans : CblasNoTrans,
		(trans_2)? CblasTrans : CblasNoTrans,
		1.0, __matrix, mat.__matrix, 0.0, result.__matrix);

	return result;
}

Matrix Matrix::matmul(Matrix &mat)
{
	return matmul(mat, false, false);
}

Matrix Matrix::T_matmul(Matrix &mat)
{
	return matmul(mat, true, false);
}

Matrix Matrix::matmul_T(Matrix &mat)
{
	return matmul(mat, false, true);
}

Matrix 	Matrix::add_vector(Vector& vec)
{
	if (vec.size() != __size.first && vec.size() != __size.second) {
		throw std::domain_error(
			"Dimension of vector does not match any dimensions "
			"of the matrix!"
			);
	}
	Matrix result = Matrix(__size.first, __size.second);
	gsl_matrix_float_memcpy(result.__matrix, __matrix);

	if (vec.size() == __size.first) {
		for (size_t i = 0; i < result.size().second; i++) {
			gsl_vector_float_view col = gsl_matrix_float_column(
				result.__matrix, i);
			gsl_vector_float_add(&(col.vector), vec.__vector);
		}

	} else {
		for (size_t i = 0; i < result.size().first; i++) {
			gsl_vector_float_view row = gsl_matrix_float_row(
				result.__matrix, i);
			gsl_vector_float_add(&(row.vector), vec.__vector);
		}
	}

	return result;
}

Matrix Matrix::map(float (*op)(float))
{
	Matrix result(__size.first, __size.second);
	for (size_t i = 0; i < __size.first; i++)
		for (size_t j = 0; j < __size.second; j++)
			result.set(i, j, op(get(i,j)));
	return result;
}

void Matrix::transpose()
{
	Matrix mnew(__size.second, __size.first);
	gsl_matrix_float_transpose_memcpy(mnew.__matrix, __matrix);
	set(mnew, true); // allow resize
}
