#include "layers.h"

Matrix Affine::propagate_forward(Matrix& X)
{
	__X.set(X, true); // allow resize
	return __X.matmul(__W).add_vector(__b);
}

Matrix Affine::propagate_backward(Matrix &g_out)
{
	// Compute gradients on weights
	for (size_t i = 0; i < g_out.size().first; i++) {
		Vector g = g_out.get_row(i);
		Vector x = __X.get_row(i);
		Matrix w = x.outer_product(g);
		if (i == 0) {
			__W_grad.set(w);
		} else {
			Matrix w_upd = __W_grad.add_matrix(w);
			__W_grad.set(w_upd);
		}
	}
	__W_grad.scale(1.0 / g_out.size().first);
	
	// Compute gradients on biases
	Vector newb = g_out.mean_row();
	__b_grad.set(newb);

	// Propagate the gradients w.r.t. the upstream layer
	return g_out.matmul_T(__W);
}

void Affine::update_parameters(float stepsize)
{
	// W update
	for (size_t i = 0; i < __size.first; i++)
		for (size_t j = 0; j < __size.second; j++)
			__W.set(i,j,__W.get(i,j) + stepsize * __W_grad.get(i,j));
	// b update
	for (size_t i = 0; i < __size.second; i++)
		__b.set(i, __b.get(i) + stepsize * __b_grad.get(i));

}
