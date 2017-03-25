#ifndef __RNG_H__
#define __RNG_H__

#include <gsl/gsl_rng.h>

namespace Random {
	
	void seed(unsigned long int value);
	float uniform(float min, float max);
}

#endif
