#include <iostream>
#include <../inc/sgd.h>
#include <data.h>
#include <omp.h>
using namespace std;
using namespace Eigen;

int main(int argc, char* argv[]){
	if(argc==3 || argc==4)
	{
		// - MovieIDs range from 1 to 17770 sequentially.
		// - CustomerIDs range from 1 to 2649429, with gaps. There are 480189 users.
		// Data* d = new Data("netflix","data/netflix/", 2649429+1, 17770+1); 
		
		// Data* d = new Data("netflix","data/trial/", 7+1, 3+1); 
		
		Data* d = new Data("movielens","data/ml-10M100K/", 71567+1,65133+1); 
		Sgd* s = new Sgd(d,atoi(argv[1]));
		
		if(argc==4){
			cout<<"Blockwise"<<endl;
			s->setupBlocks(atoi(argv[2]));
			cout<<"Setup "<<argv[2]<<" blocks is done"<<endl;
			cout<<"Computing error"<<endl;
			s->getBlockwiseTotalSquaredError();
			cout<<"Running blockwise for "<<argv[3]<<" iterations"<<endl;
			s->blockWiseFactorizeTasks(atoi(argv[3]));
			cout<<"Computing error"<<endl;
			s->getBlockwiseTotalSquaredError();

		} else if(argc==3){
			cout<<"Running serially"<<endl;
			cout<<"Computing error"<<endl;
			s->getTotalSquaredError();
			if(atoi(argv[2])!=0){
				s->factorize(atoi(argv[2]));
				cout<<"Computing error"<<endl;
				s->getTotalSquaredError();
			}
		}
	// } else if (argc==2) {
	} 
	else{
		cout<<"Error. Give arguments like "<<endl;
		cout<<"  num_latentdim num_blocks num_iters_block"<<endl;
		cout<<"or num_latentdim num_iters_normal"<<endl;
	}
	return 0;
}