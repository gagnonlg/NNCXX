#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unit_tests_layers
#include <boost/test/unit_test.hpp>

#include <stdexcept>

#include "layers.h"
#include "rng.h"

BOOST_AUTO_TEST_CASE(Affine_ctor_size)
{
	Affine lyr(10, 2);
	auto sz = lyr.size();
	BOOST_TEST(sz.first == 10);
	BOOST_TEST(sz.second == 2);	
}

BOOST_AUTO_TEST_CASE(Affine_propagate_forward_size)
{
	Affine lr(10, 2);
	Matrix X(100, 10);

	Matrix result = lr.propagate_forward(X);

	BOOST_TEST(result.size().first == 100);
	BOOST_TEST(result.size().second == 2);
}

BOOST_AUTO_TEST_CASE(Affine_propagate_forward_badsize)
{
	Affine lr(10, 2);
	Matrix X(100, 11);

	BOOST_CHECK_THROW(lr.propagate_forward(X), std::domain_error);
}

BOOST_AUTO_TEST_CASE(Affine_propagate_forward_identity)
{
	Affine lyr(2, 2);
	Matrix& W = lyr.get_weights();
	for (size_t i = 0; i < W.size().first; i++)
		for (size_t j = 0; j < W.size().second; j++)
			W.set(i, j, (i == j)? 1 : 0);

	Random::seed(28082);
	Matrix X = Matrix::uniform(10, 2);
	Matrix prod = lyr.propagate_forward(X);
	for (size_t i = 0; i < X.size().first; i++)
		for (size_t j = 0; j < X.size().second; j++)
			BOOST_TEST(X.get(i, j) == prod.get(i, j));			
}

BOOST_AUTO_TEST_CASE(Affine_propagate_forward_computed)
{
	Affine lyr(2, 2);
	Matrix& W = lyr.get_weights();
	W.set(0,0, 1);
	W.set(0,1, 2);
	W.set(1,0, 3);
	W.set(1,1, 4);

	Matrix X = Matrix(1, 2);
	X.set(0, 0, 10);
	X.set(0, 1, 100);
	Matrix prod = lyr.propagate_forward(X);

	BOOST_TEST(prod.get(0,0) == (1*10 + 100*3));
	BOOST_TEST(prod.get(0,1) == (2*10 + 100*4));
}
