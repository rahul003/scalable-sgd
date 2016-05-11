#ifndef DATA_H_
#define DATA_H_

#include <iostream>
#include <string>
#include <map>
#include <../libs/eigen/Eigen/SparseCore>
#include <../libs/eigen/Eigen/Dense>
using namespace std;
typedef Eigen::Triplet<double> T;

class Data{
	int num_users_;
	int num_movies_;
	int num_latent_;
	int N_;
public:
	Eigen::SparseMatrix<double>* V_;
	double** W_;
	double** H_;

	Data();
	Data(int);
	Data(std::string type, std::string file_path, int num_users, int num_movies);
	
	int getNetflixNumRatings(string);//, map<string, int>& );
	void readNetflixTriplets(string filename, vector<T>& triplets);
	int getMovieLensNumRatings();
	
	void shuffleV();
	void initializeFactors(int);
	
	double dotProduct(int Wi, int Hi);
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

#endif