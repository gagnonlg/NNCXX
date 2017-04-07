#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unit_tests_layers
#include <boost/test/unit_test.hpp>

#include "layers.h"

BOOST_AUTO_TEST_CASE(Affine_ctor_size)
{
	Affine lyr(10, 2);
	auto sz = lyr.size();
	BOOST_TEST(sz.first == 10);
	BOOST_TEST(sz.second == 2);	
}
