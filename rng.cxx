#include <ctime>
#include <fstream>
#include "rng.h"
#include <iostream>
#include <cstring>

static gsl_rng* rnginit()
{
	unsigned long int seed;

	std::ifstream urandom("/dev/urandom");
	if (urandom.good())
		urandom.read(reinterpret_cast<char*>(&seed), sizeof(seed));
	else
		seed = static_cast<unsigned long int>(std::time(nullptr));

	gsl_rng *rng = gsl_rng_alloc(gsl_rng_mt19937);
	gsl_rng_set(rng, seed);

	return rng;
}

static gsl_rng *GLOBAL_RNG = rnginit();

namespace Random {
void seed(unsigned long int value)
{
	gsl_rng_set(GLOBAL_RNG, value);
}

float uniform(float min, float max)
{
	float sample = static_cast<float>(gsl_rng_uniform(GLOBAL_RNG));
	return sample * (max - min) + min;
}
}
