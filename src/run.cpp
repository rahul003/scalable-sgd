#include <iostream>
#include <../inc/sgd.h>

int main(){
	// - MovieIDs range from 1 to 17770 sequentially.
// - CustomerIDs range from 1 to 2649429, with gaps. There are 480189 users.
	srand(time(NULL));
	// Data* d = new Data("netflix","data/trial/", 7+1, 3+1); 
	Data* d = new Data("movielens","data/ml-10M100K/", 71567+1,65133+1); 
	// Data* d = new Data("netflix","data/netflix/", 2649429+1, 17770+1); 
	Sgd* s = new Sgd(d,10000);
	s->factorize();
	// vector<T> triples;
	// triples.push_back(T(0,1,3));
	// triples.push_back(T(1,0,22));
	// triples.push_back(T(1,4,17));
	// triples.push_back(T(2,0,7));
	// triples.push_back(T(2,1,5));
	// triples.push_back(T(2,3,1));
	// triples.push_back(T(4,2,14));
	// triples.push_back(T(4,4,8));
	// Eigen::SparseMatrix<double> V_ = Eigen::SparseMatrix<double>(5,5);
 //    V_.setFromTriplets(triples.begin(), triples.end());
 //    cout<<V_.rows()<<" "<<V_.cols()<<endl;
 //    cout<<V_.nonZeros()<<endl;
 //    V_.makeCompressed();
}