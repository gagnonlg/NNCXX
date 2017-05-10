#include "layers.h"

Matrix Affine::propagate_forward(Matrix& X)
{
	__X.set(X, true); // allow resize
	return __X.matmul(__W).add_vector(__b);
}
	
