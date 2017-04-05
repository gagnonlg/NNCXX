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
