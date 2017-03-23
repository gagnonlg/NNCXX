#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <cstddef>

#include <gsl/gsl_matrix_float.h>
#include <gsl/gsl_vector.h>

class AffineMap {
public:
	AffineMap(size_t n_in, size_t n_out) :
		_n_in(n_in),
		_n_out(n_out),
		_cache(nullptr),
		_W(gsl_matrix_float_calloc(n_in, n_out)),
		_b(gsl_vector_float_calloc(n_out)) {}
	gsl_matrix_float* forward_propagate(gsl_matrix_float *X);
private:
	size_t _n_in, _n_out;
	gsl_matrix_float *_cache;
	gsl_matrix_float *_W;
	gsl_vector_float *_b;

	void _resize_cache(size_t n_rows);
};

#endif
