#include <../inc/sgd.h>
using namespace std;

Sgd::Sgd(Data* d, int num_latent){
	d_ = d;
	num_latent_ = num_latent;
	d->initializeFactors(num_latent);
}

void Sgd::factorize(){
	d_->computeSquaredLoss();
}
