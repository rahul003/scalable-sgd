#include <iostream>
#include <string>
#include <../libs/eigen/Eigen/SparseCore>
#include <../libs/eigen/Eigen/Dense>
#include <map>
using namespace std;
typedef Eigen::Triplet<double> T;

class Data{
	// double** product_;

	int num_users_;
	int num_movies_;
	int num_latent_;
	int N_;
public:
	Eigen::SparseMatrix<double>* V_;
	double** W_;
	double** H_;

	Data();
	Data(std::string type, std::string file_path, int num_users, int num_movies);
	int getNetflixNumRatings(string);//, map<string, int>& );
	void readNetflixTriplets(string filename, vector<T>& triplets);
	void initializeFactors(int);
	// void multiplyWH();
	double dotProduct(int Wi, int Hi);
	int getMovieLensNumRatings();
	
	double getW(int i,int k);
	double getH(int k,int j);
	void updateW(int i, int k, double dW);
	void updateH(int k, int j, double dH);
	int getNumUsers();
	int getNumMovies();
	int getN();
	int getVrows();
	int getVcols();
	void printV();
	void printW();
	void printH();

};