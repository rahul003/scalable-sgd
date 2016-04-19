#include <iostream>
#include <string>
#include <../libs/eigen/Eigen/SparseCore>
#include <../libs/eigen/Eigen/Dense>
#include <map>
using namespace std;
typedef Eigen::Triplet<double> T;

class Data{
	Eigen::SparseMatrix<double> V_;
	double** W_;
	double** H_;
	double** product_;

	int num_users_;
	int num_movies_;
	int num_latent_;
public:
	Data();
	Data(std::string file_path, int num_users, int num_movies);
	void loadNetflixMetadata(string, int&);//, map<string, int>& );
	void readAsTriplets(string filename, vector<T>& triplets);
	void initializeFactors(int);
	void multiplyWH();
	void computeSquaredLoss();
	
	int getNumUsers();
	int getNumMovies();
};