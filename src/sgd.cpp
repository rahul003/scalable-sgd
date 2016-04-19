#include <../inc/sgd.h>
#include <../inc/utils.h>
using namespace std;

Sgd::Sgd(Data* d, int num_latent){
	d_ = d;
	num_latent_ = num_latent;
	d->initializeFactors(num_latent);
	alpha_ = 0.0001;
}

double Sgd::getSquaredError(int row, int col, int value){
    double e = (value-(d_->dotProduct(row,col)));
    return e*e;
}

double Sgd::getTotalError(){

	return 0.0;
}

void Sgd::factorize(){
	for(int iter=0; iter<1000; iter++){
		for (int a = 0; a < d_->V_->outerSize(); ++a){
	    	for (Eigen::SparseMatrix<double>::InnerIterator it(*(d_->V_), a); it; ++it){
	    		int i=it.row();
	    		int j=it.col();
	    		double e = getSquaredError(i,j, it.value());
	    		//gradient descent
	    		for(int k=0; k<num_latent_;k++){
	    			double Wik = d_->getW(i,k);
	    			double Hkj = d_->getH(k,j);
	    			d_->updateW(i,k,(2*alpha_*e*Hkj));
	    			d_->updateH(k,j,(2*alpha_*e*Wik));
	    		}
		    }
		}
		double E = getTotalError();
		VDUMP(E);
	}
}
