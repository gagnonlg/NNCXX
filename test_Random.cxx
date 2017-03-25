#include <iostream>

#include "rng.h"

int main()
{
	Random::seed(5485);
	std::cout << Random::uniform(0,1) << std::endl;
}
