#include <../inc/sgd.h>
#include <../inc/utils.h>
#include <ctime>
#include <algorithm>
#include <omp.h>
using namespace std;

#define DEBUG

#ifdef DEBUG
#  define D(x) x
#else
#  define D(x)
#endif // DEBUG

Sgd::Sgd(Data* d, int num_latent){
	d_ = d;
	num_latent_ = num_latent;
	d->initializeFactors(num_latent);
	alpha_ = 0.0002;
	beta_ = 0.02;
	stepsize_ = 1;
	num_steps_ = 0;
	loadNonZeros();
}

double Sgd::getError(int row, int col, int value){
    double e = (value-(d_->dotProduct(row,col)));
    // cout<<value<<" "<<d_->dotProduct(row,col)<<" " <<e<<endl;
    return e;
}

double Sgd::getTotalSquaredError(){
	double totalError=0;
	double start_time = omp_get_wtime();
	for (int a = 0; a < d_->V_->outerSize(); ++a){
    	for (Eigen::SparseMatrix<double>::InnerIterator it(*(d_->V_), a); it; ++it){
    		int i=it.row();
    		int j=it.col();
    		double e = getError(i,j, it.value());
    		totalError += e*e;
	    }
    	// cout<<totalError<<endl;
	}
	double timetaken =  omp_get_wtime() - start_time;
	VDUMP(totalError);
	VDUMP(timetaken);
	return totalError;
}

void Sgd::updateStepsize(){
	// stepsize_ = 1/pow(num_steps_,alpha_);
}

double Sgd::getStepsize(){	
	return stepsize_;
}

void Sgd::incrementSteps(){
	num_steps_++;
}

void Sgd::loadRandomSample(int& i, int& j){
	int randomIndex = rand() % nonzeros_.size();
	pair<int, int> cell = nonzeros_[randomIndex];
	i = cell.first;
	j = cell.second;
}

// change one iteration to all samples in it
void Sgd::factorize(int iterations){
	double start_time = omp_get_wtime();
	int N = d_->getN();
	// for(int iter=0; iter<iterations; iter++){		
	for(int iter=0; iter<nonzeros_.size(); iter++){		
		int i,j;
		loadRandomSample(i,j);
		double e = getError(i,j, d_->V_->coeffRef(i,j));
		for(int k=0; k<num_latent_;k++){
			double Wik = d_->getW(i,k);
			double Hkj = d_->getH(k,j);

			// d_->updateW(i,k,(N*alpha_*e));
			// d_->updateH(k,j,(N*alpha_*e));
			
			double dW = ((2*alpha_*e*Hkj) - (beta_*Wik));
			double dH = ((2*alpha_*e*Wik) - (beta_*Hkj));

			d_->updateW(i,k,dW);
			d_->updateH(k,j,dH);
		}

		if(iter%10000==0)
			cout<<iter<<endl;
		// incrementSteps();
		// updateStepsize();
		// updateAlpha();
	}
	double time1 = omp_get_wtime() - start_time;
	cout<<time1<<"s for "<<iterations<<" iters"<<endl;
}

void Sgd::loadNonZeros(){
	for (int a = 0; a < d_->V_->outerSize(); ++a){
    	for (Eigen::SparseMatrix<double>::InnerIterator it(*(d_->V_), a); it; ++it){
    		int i=it.row();
    		int j=it.col();
    		pair<int, int> cell (i,j);
    		nonzeros_.push_back(cell);
	    }
	}
}
