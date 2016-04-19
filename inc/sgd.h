#include <iostream>
#include <data.h>

//SGD For MATRIX FACTORIZATION
class Sgd{
	int num_latent_;
	Data d_;

public:
	Sgd(Data d, int num_latent);
	void factorize();
	void printRMSE();
};