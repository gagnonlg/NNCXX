#include <cstddef>

#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix_float.h>
#include <gsl/gsl_vector.h>

#include "network.h"

void AffineMap::_resize_cache(size_t n_rows)
{
	if (_cache && (n_rows != _cache->size1)) {
		gsl_matrix_float_free(_cache);
		_cache = nullptr;
	}	
	if (!_cache)
		_cache = gsl_matrix_float_alloc(n_rows, _n_out);
}

gsl_matrix_float* AffineMap::forward_propagate(gsl_matrix_float *X)
{
	_resize_cache(X->size1);
	gsl_blas_sgemm(CblasNoTrans, CblasNoTrans, 1.0, X, _W, 0.0, _cache);
	for (size_t i = 0; i < _cache->size1; i++) {
		gsl_vector_float_view row = gsl_matrix_float_row(_cache, i);
		gsl_vector_float_add(&(row.vector), _b);
	}
	return _cache;
}
