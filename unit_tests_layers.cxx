#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unit_tests_layers
#include <boost/test/unit_test.hpp>

#include <cmath>
#include <stdexcept>

#include "layers.h"
#include "rng.h"

float approx_gradient(float (*f)(float), float x)
{
	float eps = 1e-5;
	return (f(x + 0.5 * eps) - f(x - 0.5 * eps)) / eps;
}


BOOST_AUTO_TEST_CASE(test_approx_gradient)
{
	BOOST_TEST(approx_gradient(std::sin, 0), std::cos(0));
	BOOST_TEST(approx_gradient(std::sin, 0.5), std::cos(0.5));
	BOOST_TEST(approx_gradient(std::sin, -2.245), std::cos(-2.245));
}

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

BOOST_AUTO_TEST_CASE(Affine_propagate_backward_badsize)
{
	Affine trans(100, 100);
	Matrix grads(10, 10);
	BOOST_CHECK_THROW(trans.propagate_backward(grads), std::domain_error);
}

BOOST_AUTO_TEST_CASE(Sanity_test)
{
	Matrix input(1,1);
	input.set(0, 0, 2);

	Affine trans(1,1);
	trans.get_weights().set(0, 0, 0.5);

	Matrix output = trans.propagate_forward(input);

	BOOST_TEST(output.get(0, 0) == 1.0);

	auto loss = [](Matrix &y) {
		return 0.5 * y.get(0, 0) * y.get(0, 0);
	};

	BOOST_TEST(loss(output) == 0.5);

	float dldw = output.get(0,0) * input.get(0,0);

	// now with finite difference
	float eps = 1e-3;
	
	Matrix w_orig(1,1);
	Matrix &w = trans.get_weights();
	w_orig.set(w);
	
	w.set(0, 0, w_orig.get(0, 0) + 0.5 * eps);
	Matrix o_up = trans.propagate_forward(input);
	float l_up = loss(o_up);

	w.set(0, 0, w_orig.get(0, 0) - 0.5 * eps);
	Matrix o_dn = trans.propagate_forward(input);
	float l_dn = loss(o_dn);

	float dldw_approx = (l_up - l_dn) / eps;

	BOOST_CHECK_CLOSE(dldw, dldw_approx, eps);
}

BOOST_AUTO_TEST_CASE(Sanity_test_2)
{
	Matrix input(1,2);
	input.set(0, 0, 2);
	input.set(0, 1, 2);

	Affine trans(2,2);
	trans.get_weights().set(0, 0, 0.5);
	trans.get_weights().set(0, 1, 0.5);
	trans.get_weights().set(1, 0, 0.5);
	trans.get_weights().set(1, 1, 0.5);

	Matrix output = trans.propagate_forward(input);

	BOOST_TEST(output.get(0, 0) == 2.0);
	BOOST_TEST(output.get(0, 1) == 2.0);

	auto loss = [](Matrix &g){
		float acc = 0;
		for (size_t i = 0; i < g.size().second; i++)
			acc += (g.get(0, i) * g.get(0, i));
		return 0.5 * acc;
	};

	BOOST_TEST(loss(output) == 4.0);

	Vector xt = input.get_row(0);
	Matrix dldw = output.get_row(0).outer_product(xt);

	BOOST_TEST(dldw.get(0,0) == 4);
	BOOST_TEST(dldw.get(0,1) == 4);
	BOOST_TEST(dldw.get(1,0) == 4);
	BOOST_TEST(dldw.get(1,1) == 4);

	// now with finite difference
	float eps = 1e-3;
	
	Matrix w_orig(2,2);
	Matrix &w = trans.get_weights();
	w_orig.set(w);

	auto approx = [&](size_t i, size_t j) {
		w.set(i,j, w_orig.get(i,j) + 0.5 * eps);
		Matrix o_up = trans.propagate_forward(input);
		float l_up = loss(o_up);
		w.set(i,j, w_orig.get(i,j) - 0.5 * eps);
		Matrix o_down = trans.propagate_forward(input);
		float l_down = loss(o_down);
		return (l_up - l_down) / eps;
	};

	Matrix w_approx(2,2);
	for (size_t i = 0; i < 2; i++)
		for (size_t j = 0; j < 2; j++)
			BOOST_CHECK_CLOSE(dldw.get(i,j), approx(i,j), 0.1);
}



BOOST_AUTO_TEST_CASE(Affine_propagate_backward)
{
	Matrix input(1,2);
	input.set(0, 0, 2);
	input.set(0, 1, 2);

	Affine trans(2,2);
	trans.get_weights().set(0, 0, 0.5);
	trans.get_weights().set(0, 1, 0.5);
	trans.get_weights().set(1, 0, 0.5);
	trans.get_weights().set(1, 1, 0.5);

	Matrix output = trans.propagate_forward(input);

	BOOST_TEST(output.get(0, 0) == 2.0);
	BOOST_TEST(output.get(0, 1) == 2.0);

	auto loss = [](Matrix &g){
		float acc = 0;
		for (size_t i = 0; i < g.size().second; i++)
			acc += (g.get(0, i) * g.get(0, i));
		return 0.5 * acc;
	};

	BOOST_TEST(loss(output) == 4.0);

	trans.propagate_backward(output);
	Matrix & dldw_ = trans.get_weights_gradients();
	Matrix dldw(2,2);
	dldw.set(dldw_);

	BOOST_TEST(dldw.get(0,0) == 4);
	BOOST_TEST(dldw.get(0,1) == 4);
	BOOST_TEST(dldw.get(1,0) == 4);
	BOOST_TEST(dldw.get(1,1) == 4);

	// now with finite difference
	float eps = 1e-3;
	
	Matrix w_orig(2,2);
	Matrix &w = trans.get_weights();
	w_orig.set(w);

	auto approx = [&](size_t i, size_t j) {
		w.set(i,j, w_orig.get(i,j) + 0.5 * eps);
		Matrix o_up = trans.propagate_forward(input);
		float l_up = loss(o_up);
		w.set(i,j, w_orig.get(i,j) - 0.5 * eps);
		Matrix o_down = trans.propagate_forward(input);
		float l_down = loss(o_down);
		return (l_up - l_down) / eps;
	};

	Matrix w_approx(2,2);
	for (size_t i = 0; i < 2; i++)
		for (size_t j = 0; j < 2; j++)
			BOOST_CHECK_CLOSE(dldw.get(i,j), approx(i,j), 0.1);
}

BOOST_AUTO_TEST_CASE(Affine_propagate_backward_shape)
{
	size_t ni = 2;
	size_t no = 3;
	Matrix input = Matrix::uniform(1,ni);
	Affine trans(ni,no);
	Matrix output = trans.propagate_forward(input);
	BOOST_TEST(output.size().first == 1);
	BOOST_TEST(output.size().second == no);
	trans.propagate_backward(output);
	Matrix & dldw_ = trans.get_weights_gradients();
	BOOST_TEST(dldw_.size().first == ni);
	BOOST_TEST(dldw_.size().second == no);
}

BOOST_AUTO_TEST_CASE(Affine_propagate_backward_2)
{

	size_t ni = 2;
	size_t no = 3;
        
	Matrix input = Matrix::uniform(1,ni);
	Affine trans(ni,no);
	Matrix output = trans.propagate_forward(input);

	auto loss = [](Matrix &g){
		float acc = 0;
		for (size_t i = 0; i < g.size().second; i++)
			acc += (g.get(0, i) * g.get(0, i));
		return 0.5 * acc;
	};

	trans.propagate_backward(output);
	Matrix & dldw_ = trans.get_weights_gradients();
	Matrix dldw(ni,no);
	dldw.set(dldw_);

	// now with finite difference
	float eps = 1e-3;
	
	Matrix w_orig(ni,no);
	Matrix &w = trans.get_weights();
	w_orig.set(w);

	auto approx = [&](size_t i, size_t j) {
		w.set(i,j, w_orig.get(i,j) + 0.5 * eps);
		Matrix o_up = trans.propagate_forward(input);
		float l_up = loss(o_up);
		w.set(i,j, w_orig.get(i,j) - 0.5 * eps);
		Matrix o_down = trans.propagate_forward(input);
		float l_down = loss(o_down);
		w.set(w_orig);
		return (l_up - l_down) / eps;
	};

	for (size_t i = 0; i < ni; i++)
		for (size_t j = 0; j < no; j++)
			BOOST_CHECK_CLOSE(dldw.get(i,j), approx(i,j), 0.1);
}

BOOST_AUTO_TEST_CASE(Affine_propagate_backward_b)
{

	size_t ni = 2;
	size_t no = 3;
        
	Matrix input = Matrix::uniform(1,ni);
	Affine trans(ni,no);
	for (size_t i = 0; i < no; i++)
		trans.get_biases().set(i, Random::uniform(-0.1,0.1)); 
	Matrix output = trans.propagate_forward(input);

	auto loss = [](Matrix &g){
		float acc = 0;
		for (size_t i = 0; i < g.size().second; i++)
			acc += (g.get(0, i) * g.get(0, i));
		return 0.5 * acc;
	};

	trans.propagate_backward(output);
	Vector & dldb_ = trans.get_biases_gradients();
	Vector dldb(no);
	dldb.set(dldb_);

	// now with finite difference
	float eps = 1e-3;
	
	Vector b_orig(no);
	Vector &b = trans.get_biases();
	b_orig.set(b);

	auto approx = [&](size_t i) {
		b.set(i, b_orig.get(i) + 0.5 * eps);
		Matrix o_up = trans.propagate_forward(input);
		float l_up = loss(o_up);
		b.set(i, b_orig.get(i) - 0.5 * eps);
		Matrix o_down = trans.propagate_forward(input);
		float l_down = loss(o_down);
		b.set(b_orig);
		return (l_up - l_down) / eps;
	};

	for (size_t i = 0; i < no; i++)
		BOOST_CHECK_CLOSE(dldb.get(i), approx(i), 0.1);
}
