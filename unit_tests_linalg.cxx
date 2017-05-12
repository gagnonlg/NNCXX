#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unit_tests_linalg
#include <boost/test/unit_test.hpp>

#include "linalg.h"
#include "rng.h"

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

BOOST_AUTO_TEST_CASE(Matrix_uniform, * boost::unit_test::tolerance(0.00001))
{
	Random::seed(32706);
	Matrix matrix = Matrix::uniform(2, 3, -10, 10);
	BOOST_TEST(matrix.get(0,0) == -6.05539);
	BOOST_TEST(matrix.get(0,1) == 6.16077);
	BOOST_TEST(matrix.get(0,2) == -3.51725);
	BOOST_TEST(matrix.get(1,0) == -3.05564);
	BOOST_TEST(matrix.get(1,1) == 7.152);
	BOOST_TEST(matrix.get(1,2) == -9.27988);
}

BOOST_AUTO_TEST_CASE(Matrix_matmul_size)
{
	Matrix m1(2, 3);
	Matrix m2(3, 4);
	Matrix m3 = m1.matmul(m2);
	auto shp = m3.size();
	BOOST_TEST(shp.first == 2);
	BOOST_TEST(shp.second == 4);
}

BOOST_AUTO_TEST_CASE(Matrix_matmul_badsize)
{
	Matrix m1(2, 3);
	Matrix m2(4, 5);
	BOOST_CHECK_THROW(m1.matmul(m2), std::domain_error);
}

BOOST_AUTO_TEST_CASE(Matrix_matmul_mult)
{
	Matrix m1(2, 3);
	m1.set(0,0,1);
	m1.set(0,1,2);
	m1.set(0,2,3);
	m1.set(1,0,4);
	m1.set(1,1,5);
	m1.set(1,2,6);
	
	Matrix m2(3, 2);
	m2.set(0,0,1);
	m2.set(0,1,2);
	m2.set(1,0,10);
	m2.set(1,1,20);
	m2.set(2,0,100);
	m2.set(2,1,200);

	Matrix m3 = m1.matmul(m2);
	BOOST_TEST(m3.get(0,0) == (1*1 + 2*10 + 3*100));
	BOOST_TEST(m3.get(0,1) == (1*2 + 2*20 + 3*200));
	BOOST_TEST(m3.get(1,0) == (4*1 + 5*10 + 6*100));
	BOOST_TEST(m3.get(1,1) == (4*2 + 5*20 + 6*200));
}

BOOST_AUTO_TEST_CASE(Matrix_matmul_T1)
{
	Matrix m1(2, 3);
	m1.set(0,0,1);
	m1.set(0,1,2);
	m1.set(0,2,3);
	m1.set(1,0,4);
	m1.set(1,1,5);
	m1.set(1,2,6);
	m1.transpose();
	
	Matrix m2(3, 2);
	m2.set(0,0,1);
	m2.set(0,1,2);
	m2.set(1,0,10);
	m2.set(1,1,20);
	m2.set(2,0,100);
	m2.set(2,1,200);

	Matrix m3 = m1.T_matmul(m2);
	BOOST_TEST(m3.get(0,0) == (1*1 + 2*10 + 3*100));
	BOOST_TEST(m3.get(0,1) == (1*2 + 2*20 + 3*200));
	BOOST_TEST(m3.get(1,0) == (4*1 + 5*10 + 6*100));
	BOOST_TEST(m3.get(1,1) == (4*2 + 5*20 + 6*200));
}

BOOST_AUTO_TEST_CASE(Matrix_matmul_T2)
{
	Matrix m1(2, 3);
	m1.set(0,0,1);
	m1.set(0,1,2);
	m1.set(0,2,3);
	m1.set(1,0,4);
	m1.set(1,1,5);
	m1.set(1,2,6);
	
	Matrix m2(3, 2);
	m2.set(0,0,1);
	m2.set(0,1,2);
	m2.set(1,0,10);
	m2.set(1,1,20);
	m2.set(2,0,100);
	m2.set(2,1,200);
	m2.transpose();

	Matrix m3 = m1.matmul_T(m2);
	BOOST_TEST(m3.get(0,0) == (1*1 + 2*10 + 3*100));
	BOOST_TEST(m3.get(0,1) == (1*2 + 2*20 + 3*200));
	BOOST_TEST(m3.get(1,0) == (4*1 + 5*10 + 6*100));
	BOOST_TEST(m3.get(1,1) == (4*2 + 5*20 + 6*200));
}



BOOST_AUTO_TEST_CASE(Matrix_add_vector_size)
{
	Matrix mat(5,6);
	Vector vec(6);
	Matrix added = mat.add_vector(vec);

	auto added_sz = added.size();
	auto mat_sz = mat.size();
	
	BOOST_TEST(added_sz.first == mat_sz.first);
	BOOST_TEST(added_sz.second == mat_sz.second);
}

BOOST_AUTO_TEST_CASE(Matrix_add_vector_badsize)
{
	Matrix mat(5,6);
	Vector vec1(4);
	Vector vec2(7);

	BOOST_CHECK_THROW(mat.add_vector(vec1), std::domain_error);
	BOOST_CHECK_THROW(mat.add_vector(vec2), std::domain_error);
}


BOOST_AUTO_TEST_CASE(Matrix_add_vector_row)
{
	Matrix mat(5,6);
	Vector vec(6);
	for (size_t i = 0; i < 6; i++)
		vec.set(i, i);
	
	Matrix added = mat.add_vector(vec);

	for (size_t i = 0; i < 5; i++)
		for (size_t j = 0; j < 6; j++)
			BOOST_TEST((mat.get(i,j) + j) == added.get(i, j));
		
}

BOOST_AUTO_TEST_CASE(Matrix_add_vector_col)
{
	Matrix mat(5,6);
	Vector vec(5);
	for (size_t i = 0; i < 5; i++)
		vec.set(i, i);
	
	Matrix added = mat.add_vector(vec);

	for (size_t i = 0; i < 5; i++)
		for (size_t j = 0; j < 6; j++)
			BOOST_TEST((mat.get(i,j) + i) == added.get(i, j));
		
}

float mult100(float e)
{
	return e * 100;
}

BOOST_AUTO_TEST_CASE(Matrix_map_size)
{
	Matrix mat(5,6);
	Matrix result = mat.map(mult100);
	auto rsize = result.size();
	BOOST_TEST(rsize.first == 5);
	BOOST_TEST(rsize.second == 6);
}

BOOST_AUTO_TEST_CASE(Matrix_map_result)
{
	Random::seed(30383);
	Matrix mat = Matrix::uniform(5,6);
	Matrix result = mat.map(mult100);
	for (size_t i = 0; i < 5; i++)
		for (size_t j = 0; j < 6; j++)
			BOOST_TEST(result.get(i,j) == mat.get(i,j) * 100);
}

BOOST_AUTO_TEST_CASE(Matrix_set_matrix_resize)
{
	Random::seed(30383);
	Matrix mat1 = Matrix::uniform(5,6);
	Matrix mat2 = Matrix::uniform(4,7);

	mat1.set(mat2, true);
	BOOST_TEST(mat1.size().first == 4);
	BOOST_TEST(mat1.size().second == 7);

	for (size_t i = 0; i < 4; i++)
		for (size_t j = 0; j < 7; j++)
			BOOST_TEST(mat1.get(i,j) == mat2.get(i,j));

}

BOOST_AUTO_TEST_CASE(Matrix_set_matrix_noresize)
{
	Random::seed(30383);
	Matrix mat1 = Matrix::uniform(5,6);
	Matrix mat2 = Matrix::uniform(4,7);
	
	BOOST_CHECK_THROW(mat1.set(mat2), std::domain_error);
}



BOOST_AUTO_TEST_CASE(Matrix_set_matrix)
{
	Random::seed(30383);
	Matrix mat1 = Matrix::uniform(5,6);
	Matrix mat2 = Matrix::uniform(5,6);

	mat1.set(mat2);

	for (size_t i = 0; i < 5; i++)
		for (size_t j = 0; j < 6; j++)
			BOOST_TEST(mat1.get(i,j) == mat2.get(i,j));

}

BOOST_AUTO_TEST_CASE(Matrix_transpose)
{
	Random::seed(30383);
	Matrix mat1 = Matrix::uniform(5,6);
	Matrix mat2(5,6);
	mat2.set(mat1);

	mat1.transpose();

 	for (size_t i = 0; i < 5; i++)
		for (size_t j = 0; j < 6; j++)
			BOOST_TEST(mat1.get(j,i) == mat2.get(i,j));

}

BOOST_AUTO_TEST_CASE(Matrix_get_row_bad_index)
{
	Matrix mat = Matrix(10, 4);
	BOOST_CHECK_THROW(mat.get_row(101), std::domain_error);
}

BOOST_AUTO_TEST_CASE(Matrix_get_row_size)
{
	Matrix mat = Matrix(10, 4);
	BOOST_TEST(mat.get_row(5).size() == 4);
}

BOOST_AUTO_TEST_CASE(Matrix_get_row_values)
{
	Random::seed(31113);
	Matrix mat = Matrix::uniform(10,4);
	Vector row = mat.get_row(0);
	for (size_t i = 0; i < 4; i++)
		BOOST_TEST(row.get(i) == mat.get(0, i));
}

BOOST_AUTO_TEST_CASE(Vector_outer_product_size)
{
	Vector v1(2);
	Vector v2(3);
	Matrix m = v1.outer_product(v2);
	BOOST_TEST(m.size().first == 2);
	BOOST_TEST(m.size().second == 3);
}

BOOST_AUTO_TEST_CASE(Vector_outer_product_computed)
{
	Vector v1(2);
	v1.set(0, 10);
	v1.set(1, 100);
	
	Vector v2(3);
	v2.set(0, 1);
	v2.set(1, 2);
	v2.set(2, 3);
	
	Matrix m = v1.outer_product(v2);

	BOOST_TEST(m.get(0,0) == (1 * 10));
	BOOST_TEST(m.get(1,0) == (1 * 100));	
	BOOST_TEST(m.get(0,1) == (2 * 10));
	BOOST_TEST(m.get(1,1) == (2 * 100));	
	BOOST_TEST(m.get(0,2) == (3 * 10));
	BOOST_TEST(m.get(1,2) == (3 * 100));	
}
