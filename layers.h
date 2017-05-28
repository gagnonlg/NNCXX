#include <cmath>

#include "linalg.h"

class Layer {
public:
	Matrix propagate_forward(Matrix& input);
	Matrix propagate_backward(Vector& gradient);
	void update_parameters(float stepsize);
};

class Affine : public Layer {
public:
	Affine(size_t fan_in, size_t fan_out) :
		__size(std::make_pair(fan_in, fan_out)),
	        __X(Matrix(fan_in, fan_out)),
		__W(Matrix::uniform(fan_in,
				    fan_out,
				    -1.0 / std::sqrt(fan_in + fan_out),
				    1.0 / std::sqrt(fan_in + fan_out))),
 	        __W_grad(Matrix(fan_in, fan_out)),
		__b(Vector(fan_out)),
 		__b_grad(Vector(fan_out))
		{};
	const std::pair<size_t, size_t>& size() { return __size; };

	Matrix propagate_forward(Matrix&);
	Matrix propagate_backward(Matrix&);
	void update_parameters(float stepsize);

	Matrix & get_weights() { return __W; };
	Matrix & get_weights_gradients() { return __W_grad; };
	Vector & get_biases() { return __b; };
	Vector & get_biases_gradients() { return __b_grad; };
private:
	std::pair<size_t, size_t> __size;
	Matrix __X;
	Matrix __W;
	Matrix __W_grad;
	Vector __b;
	Vector __b_grad;
};
