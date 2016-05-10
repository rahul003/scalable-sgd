#include <iostream>
#include <../inc/sgd.h>
#include <data.h>
#include <omp.h>
using namespace std;
using namespace Eigen;

int main(int argc, char* argv[]){
	// srand(time(NULL));
	if(argc==5 || argc==6)
	{
		// - MovieIDs range from 1 to 17770 sequentially.
		// - CustomerIDs range from 1 to 2649429, with gaps. There are 480189 users.
		// Data* d = new Data("netflix","data/netflix/", 2649429+1, 17770+1); 
		
		// Data* d = new Data("netflix","data/trial/", 7+1, 3+1); 
		
		Data* d = new Data("movielens","data/ml-10M100K/", 71567+1,65133+1); 
		
		Sgd* s = new Sgd(d,atoi(argv[1]));
		
		
		if(argc==6){
			//to do separately
			if(atoi(argv[5])==1){
				// s->getBlockwiseTotalSquaredError();
				s->setupBlocks(atoi(argv[2]));
				cout<<"setup "<<argv[2]<<" blocks is done"<<endl;
				cout<<"running blockwise for "<<argv[3]<<" iterations"<<endl;
				s->blockWiseFactorizeTasks(atoi(argv[3]));
				cout<<"computing error"<<endl;
				s->getBlockwiseTotalSquaredError();
			} else{
				// s->getBlockwiseTotalSquaredError();
				s->factorize(atoi(argv[4]));
				s->getBlockwiseTotalSquaredError();
			}
		} else{
			s->setupBlocks(atoi(argv[2]));
			cout<<"setup blocks is done"<<endl;
			s->blockWiseFactorizeTasks(atoi(argv[3]));
			s->getBlockwiseTotalSquaredError();
			s->factorize(atoi(argv[4]));
			s->getBlockwiseTotalSquaredError();
		}
	} else {
		cout<<"need args for num_latentdim num_blocks num_iters_block num_iters_normal (OPTIONAL-mode)"<<endl;
	}
	return 0;
	
}
