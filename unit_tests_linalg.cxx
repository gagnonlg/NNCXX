#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unit_tests_linalg
#include <boost/test/unit_test.hpp>

#include "linalg.h"

BOOST_AUTO_TEST_CASE(Vector_ctor_size)
{
	Vector vect(10);
	BOOST_TEST(vect.size() == 10);
}

BOOST_AUTO_TEST_CASE(Vector_ctor_init0)
{
	Vector vect(10);
	for (size_t i = 0; i < 10; i++)
		BOOST_TEST(vect.get(i) == 0.0);
}

BOOST_AUTO_TEST_CASE(Vector_set)
{
	Vector vect(10);
	vect.set(5, 100.0);
	BOOST_TEST(vect.get(5) == 100.0);
}

BOOST_AUTO_TEST_CASE(Vector_out_of_bound)
{
	Vector vect(10);
	BOOST_CHECK_THROW(vect.get(11), std::runtime_error);
	BOOST_CHECK_THROW(vect.set(11, 0.0), std::runtime_error);	
}

BOOST_AUTO_TEST_CASE(Matrix_ctor_size)
{
	Matrix matrix(10, 20);
	std::pair<size_t, size_t> size = matrix.size();
	BOOST_TEST(size.first == 10);
	BOOST_TEST(size.second == 20);
}

BOOST_AUTO_TEST_CASE(Matrix_ctor_init0)
{
	Matrix matrix(10, 20);
	std::pair<size_t, size_t> size = matrix.size();
	for (size_t i = 0; i < size.first; i++)
		for (size_t j = 0; j < size.second; j++)
			BOOST_TEST(matrix.get(i, j) == 0.0);
}

BOOST_AUTO_TEST_CASE(Matrix_ctor_set)
{
	Matrix matrix(10, 20);
	matrix.set(5, 6, 56);
	BOOST_TEST(matrix.get(5, 6) == 56);
}

BOOST_AUTO_TEST_CASE(Matrix_out_of_bound)
{
	Matrix matrix(10, 20);
	// get, first oob
	BOOST_CHECK_THROW(matrix.get(10, 0), std::runtime_error);
	// get, second oob
	BOOST_CHECK_THROW(matrix.get(0, 20), std::runtime_error);
	// get, both oob
	BOOST_CHECK_THROW(matrix.get(10, 20), std::runtime_error);
	// set, first oob
	BOOST_CHECK_THROW(matrix.set(10, 0, 0), std::runtime_error);
	// set, second oob
	BOOST_CHECK_THROW(matrix.set(0, 20, 0), std::runtime_error);
	// set, both oob
	BOOST_CHECK_THROW(matrix.set(10, 20, 0), std::runtime_error);
}
