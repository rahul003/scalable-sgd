#include <iostream>
#include <data.h>

//SGD For MATRIX FACTORIZATION
class Sgd{
	int num_latent_;
	Data* d_;
	double alpha_;
public:
	Sgd(Data* d, int num_latent);
	double getSquaredError(int row, int col, int value);
	double getTotalSquaredError();
	void loadRandomSample(int& i, int& j);
	void factorize();
	double getAlpha();
	void updateAlpha();
};