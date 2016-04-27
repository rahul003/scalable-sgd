#include <../inc/sgd.h>
#include <../inc/utils.h>
#include <ctime>
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
	alpha_ = 0.0001;
}

double Sgd::getSquaredError(int row, int col, int value){
    double e = (value-(d_->dotProduct(row,col)));
    // VDUMP(e);
    return e*e;
}

double Sgd::getTotalSquaredError(){
	double totalError=0;
	for (int a = 0; a < d_->V_->outerSize(); ++a){
    	for (Eigen::SparseMatrix<double>::InnerIterator it(*(d_->V_), a); it; ++it){
    		int i=it.row();
    		int j=it.col();
    		totalError += getSquaredError(i,j, it.value());
	    }
	}
	VDUMP(totalError);
	return totalError;
}

void Sgd::loadRandomSample(int& i, int& j){

	// clock_t begin = clock();

	int rows = d_->getVrows();
	int cols = d_->getVcols();
	int r = rand_interval(0,(rows-1)*(cols-1));
	i = r/(rows-1);
	j = r%(cols-1);
	if(d_->V_->coeffRef(i,j)==0)
		loadRandomSample(i,j);

	// clock_t end = clock();
	// double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	// cout<<elapsed_secs<<"s"<<endl;
}

void Sgd::updateAlpha(){
	alpha_*=0.999;
}

double Sgd::getAlpha(){	
	return alpha_;
}

void Sgd::factorize(){
	// d_->printV();
	// d_->printW();
	// d_->printH();
	clock_t begin, end, begin_i, end_i;
	double elapsed;

	cout<<"Starting factorize"<<endl;
	
	int N = d_->getN();
	for(int iter=0; iter<1000; iter++){
		begin_i = clock();
		
		int i,j;

		begin = clock();
		loadRandomSample(i,j);
		end = clock();
		elapsed = double(end - begin) / (CLOCKS_PER_SEC*1000);
		cout<<"load randomsample"<<elapsed<<"s"<<endl;

		begin = clock();
		double e = getSquaredError(i,j, d_->V_->coeffRef(i,j));
		end = clock();
		elapsed = double(end - begin) / (CLOCKS_PER_SEC*1000);
		cout<<"get error"<<elapsed<<"s"<<endl;

		for(int k=0; k<num_latent_;k++){

			double Wik = d_->getW(i,k);
			double Hkj = d_->getH(k,j);
			d_->updateW(i,k,(N*alpha_*e));
			d_->updateH(k,j,(N*alpha_*e));
		}
		updateAlpha();
		
		
		
		// if(iter%100==0){
			getTotalSquaredError();
		// }
		end_i = clock();
		elapsed = double(end_i - begin_i) /  (CLOCKS_PER_SEC*1000);
		cout<<"one full iteration"<<elapsed<<"s"<<endl;
	}
}
