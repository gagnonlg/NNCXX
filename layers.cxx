#include "layers.h"

Matrix & Affine::propagate_forward(Matrix& X)
{
        Matrix Y = X.matmul(__W).add_vector(__b);
	__Y.set(Y, true); // allow resize
	return __Y;
}
	
