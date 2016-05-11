#include <../inc/sgd.h>
#include <../inc/utils.h>
#include <ctime>
#include <algorithm>
#include <omp.h>
using namespace std;

void Sgd::setupBlocks(int num_blocks){
	num_blocks_ = num_blocks;
	int size = sqrt(num_blocks);

	int rows, cols;
	rows = d_->getVrows();
	cols = d_->getVcols();

	num_block_cols_ = cols/size;
	num_block_rows_ = rows/size;

	permuts_ = permutations(range(0,size));
	// loadNonZeroBlockwise();
	// d_->shuffleV();
	// nonzero_blockwise_.clear();
	// cout<<"shuffled"<<endl;
	loadNonZeroBlockwise();
}

void Sgd::loadNonZeroBlockwise(){
	// map<pair<int, int> , vector<pair<int, int> > > nonzero_blockwise_;
	int x = 0;
	for (int a = 0; a < d_->V_->outerSize(); ++a){
    	for (Eigen::SparseMatrix<double>::InnerIterator it(*(d_->V_), a); it; ++it){
    		int i=it.row();
    		int j=it.col();
    		pair<int, int> bl (i/num_block_rows_, j/num_block_cols_);
    		pair<int, int> cell (i,j);
    		map<pair<int, int>, vector<pair<int, int> > >::iterator search = nonzero_blockwise_.find(bl);
  			if (search == nonzero_blockwise_.end()){
  				vector<pair<int, int> > val;
  				val.push_back(cell);
    			nonzero_blockwise_[bl] = val;
  			} else{
  				nonzero_blockwise_[bl].push_back(cell);
  			}
	    }
	}

	cout<<"Number of nonzero entries in each block"<<endl;
	for(map<pair<int, int> , vector<pair<int, int> > >::iterator it = nonzero_blockwise_.begin(); it != nonzero_blockwise_.end(); it++) {
    	cout<<it->second.size()<<" ";
    	// iterator->first = key
    	// iterator->second = value
    	// Repeat if you also want to iterate through the second map.
	}
	cout<<endl;
}

int Sgd::getColOffset(int block_col){
	return block_col*num_block_cols_;
}

int Sgd::getRowOffset(int block_row){
	return block_row*num_block_rows_;
}

bool Sgd::loadRandomSampleBlock(int& i, int& j, int block_row, int block_col){
	pair<int, int> bl (block_row,block_col);
	vector<pair<int, int> > nz = nonzero_blockwise_[bl];
	if(nz.size()==0)
		return false;

	int randomIndex = rand() % nz.size();
	pair<int, int> cell = nz[randomIndex];
	i = cell.first;
	j = cell.second;
	return true;
}

double Sgd::getBlockwiseTotalSquaredError(){
	double totalError=0;
	double start_time = omp_get_wtime();
	
	int size = sqrt(num_blocks_);
	#pragma omp parallel num_threads(11) 
	{
		#pragma omp for collapse(2) reduction(+:totalError)
		for(int i=0; i<size; i++){
			for(int j=0; j<size; j++){
					pair<int, int> block (i,j);
    				double c = getBlockSquaredError(block);
    				totalError += c;
			}
		}
	}
	double timetaken = omp_get_wtime() - start_time;
	cout<<"Totalerror "<<totalError<<" , time taken to compute "<<timetaken<<endl;
	return totalError;
}

double Sgd::getBlockSquaredError(pair<int, int> block){
    double te =0;
    double e;
    vector<pair<int, int> > nz = nonzero_blockwise_[block];
    int size = nz.size();
	if(size==0)
		return te;
	
    for(int temp=0 ;temp<size; temp++){
    	int i = nonzero_blockwise_[block][temp].first;
    	int j = nonzero_blockwise_[block][temp].second;
    	 e = (d_->V_->coeffRef(i,j)-(d_->dotProduct(i,j)));
    	 te += e*e;
    }	 
    return te;
}

//i refers to row
//j refers to col
void Sgd::onBlock(int bindex_i, int bindex_j, int iters){
	pair<int, int> bl (bindex_i,bindex_j);
	int num_samples_block = nonzero_blockwise_[bl].size();
	random_shuffle(nonzero_blockwise_[bl].begin(), nonzero_blockwise_[bl].end());
	int i,j;
	for(int x=0; x<iters; x++){
		for(int it = 0; it<num_samples_block; it++){
			pair<int, int> sample = nonzero_blockwise_[bl][it];
			i = sample.first;
			j = sample.second;
			
			double e = getError(i,j, d_->V_->coeffRef(i,j));
			for(int k=0; k<num_latent_;k++){
				double Wik = d_->getW(i,k);
				double Hkj = d_->getH(k,j);

				//different variant of SGD
				// double dW = (2*eps*e*Hkj);
				// double dH = (2*eps*e*Wik);
				double dW = ((2*alpha_*e*Hkj) - (beta_*Wik));
				double dH = ((2*alpha_*e*Wik) - (beta_*Hkj));

				d_->updateW(i,k,dW);
				d_->updateH(k,j,dH);
			}
		}
	}
}

void Sgd::blockWiseFactorize(){

}
// void Sgd::blockWiseFactorizeLoops(){
// 	int Nb = (d_->getN())/num_blocks_;
// 	int num_block_size = sqrt(num_blocks_);
// 	double start_time = omp_get_wtime();
// 	int c=0;
// 	// onBlock(5,permuts_[4][5]);

// 	#pragma omp parallel num_threads(11)
// 	{	
// 		// while(c<100){
// 			for (int subepoch=0; subepoch<num_block_size; subepoch++)
// 			{
// 				//pick one stratum
// 				#pragma omp for
// 				for(int strat=0; strat<permuts_[subepoch-1].size(); strat++)
// 				{
// 					onBlock(strat,permuts_[subepoch][strat]);
// 				}
// 			}
// 			// incrementSteps();
// 			// updateStepsize();
			
// 			// #pragma omp single
// 			// {
// 			// 	cout<<c<<endl;
// 			// 	c++;
// 			// }
// 			// #pragma omp barrier
// 			// if(c%10000==0){
// 			// 	#pragma omp single
// 			// 		cout<<c<<endl;
// 		// }
		
// 	}
// 	double time1 = omp_get_wtime() - start_time;
// 	cout<<"loops"<<time1<<"s"<<endl;
// 	return;
// }


void Sgd::blockWiseFactorizeTasks(int iters){
	double start_time = omp_get_wtime();
	#pragma omp parallel num_threads(30)
	{
		#pragma omp single
		{
			for (int subepoch=0; subepoch<sqrt(num_blocks_); subepoch++)
			{
				//pick one stratum
				for(int strat=0; strat<permuts_[subepoch-1].size(); strat++)
				{
					#pragma omp task
					{
						onBlock(strat,permuts_[subepoch][strat], iters);
					}
				}
				#pragma omp taskwait
			}
			// updateAlpha();
		}		
	}
	double time1 = omp_get_wtime() - start_time;
	cout<<"tasks"<<time1<<"s"<<endl;
}