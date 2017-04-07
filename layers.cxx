#include "layers.h"

Matrix Affine::propagate_forward(Matrix& X)
{
	Matrix prod = X.matmul(__W);
	return prod.add_vector(__b);
}
	
