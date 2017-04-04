#include <cmath>
#include "NeuralNetwork"

int main(void)
{
	float lr = 0.01;
	float v_split = 0.1;
	
	size_t n_in = 1;
	size_t n_hidden = 300;
	size_t n_out = 1;
	size_t epochs = 200;
	size_t batch_size = 128;

	Dataset dataX;
	Dataset dataY;
	Dataset testX;
	Dataset testY;
	
	NeuralNetwork network;

	network.add(Layer::Affine(n_in, n_hidden));
	network.add(Layer::ReLU());
	network.add(Layer::Affine(n_hidden, n_out));

	network.train(
		dataX,
		dataY,
		Loss::MSE,
		lr,
		epochs,
		batch_size,
		v_split
		);
	
	network.save("test_sine_network.h5");

	Dataset estY = network(testX);

	Plot plot;
	plot.scatter(testX, testY, "test data");
	plot.lines(testX, estY, "NN estimation");
	plot.lines(testX, std::sin, "true data", Style::Dashed);
	plot.labels("X", "Y");
	plot.save("test_sine.png");
}
		
