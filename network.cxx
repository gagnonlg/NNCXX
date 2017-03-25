#include <cmath>
#include <cstddef>

#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix_float.h>
#include <gsl/gsl_vector.h>

#include "network.h"
#include "rng.h"


AffineMap::AffineMap(size_t n_in, size_t n_out) :
	_n_in(n_in),
	_n_out(n_out),
	_cache(nullptr),
	_W(gsl_matrix_float_calloc(n_in, n_out)),
	_b(gsl_vector_float_calloc(n_out))
{
	float bound = 1.0 / std::sqrt(_n_in + _n_out);
	for (size_t i = 0; i < _n_in; i++) {
		for (size_t j = 0; j < _n_out; j++) {
			float v = Random::uniform(-bound, bound);
			gsl_matrix_float_set(_W, i, j, v);
		}
	}

	for (size_t i = 0; i < _n_out; i++)
		gsl_vector_float_set(_b, i, 0);
}

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
