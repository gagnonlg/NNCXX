#ifndef __LINALG_H__
#define __LINALG_H__

#include <cstddef>
#include <stdexcept>
#include <string>

#include <gsl/gsl_matrix_float.h>
#include <gsl/gsl_vector_float.h>

static void gsl_error_handler(const char *reason, const char *file, int line, int gsl_errno)
{
	std::string s_reason(reason);
	std::string s_file(file);
	std::string s_line = std::to_string(line);
	std::string s_gsl_errno = std::to_string(gsl_errno);

	std::string msg =
		"GSL exception (#" + s_gsl_errno + ") at line "
		+ s_line + " of " + s_file + ": " + s_reason;
		
	throw std::runtime_error(msg.c_str());
}

static gsl_error_handler_t * __OLD_HANDLER = gsl_set_error_handler(&gsl_error_handler);

class Matrix; // forward decl to declare as friend of Vector

class Vector {
	friend class Matrix;
public:
	Vector(size_t size) :
		__size(size),
		__vector(gsl_vector_float_calloc(__size))
		{};
	~Vector() { gsl_vector_float_free(__vector); };
	size_t size() { return __size; };
	float get(size_t i) { return gsl_vector_float_get(__vector, i); };
	void set(size_t i, float v) { gsl_vector_float_set(__vector, i, v); };

private:
	size_t __size;
	gsl_vector_float *__vector;
};

class Matrix {
public:
	Matrix(size_t nrow, size_t ncol) :
		__size(std::make_pair(nrow, ncol)),
		__matrix(gsl_matrix_float_calloc(__size.first, __size.second))
		{};
	~Matrix() { gsl_matrix_float_free(__matrix); };
	const std::pair<size_t, size_t>& size() { return __size; };
	float get(size_t i, size_t j) { return gsl_matrix_float_get(__matrix, i, j); };
	void set(size_t i, size_t j, float v) { gsl_matrix_float_set(__matrix, i, j, v); };
	void set(Matrix& mat, bool allow_resize = false);
	Matrix matmul(Matrix&);
	Matrix add_vector(Vector&);
	Matrix map(float (*)(float));

	static Matrix uniform(size_t nrow, size_t ncol, float low = 0.0, float high = 1.0);
private:
	std::pair<size_t, size_t> __size;
	gsl_matrix_float *__matrix;
};

#endif
