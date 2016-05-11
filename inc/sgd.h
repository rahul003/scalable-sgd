#ifndef SGD_H_
#define SGD_H_

#include <iostream>
#include <data.h>

//SGD For MATRIX FACTORIZATION
class Sgd{
	int num_latent_;
	Data* d_;
	double alpha_;
	double stepsize_;
	double beta_;
	int num_steps_;
	int num_blocks_;
	int num_block_cols_;
	int num_block_rows_;
	vector<vector<int> > permuts_;
	map<pair<int, int> , vector<pair<int, int> > > nonzero_blockwise_;
	vector<pair<int, int> > nonzeros_;
public:
	Sgd(Data* d, int num_latent);

	void setupBlocks(int );
	
	int getRowOffset(int block_row);
	int getColOffset(int block_col);
	void onBlock(int b_i, int b_j, int iters);
	void loadNonZeroBlockwise();
	
	void shuffleData();
	void loadNonZeros();
	double getError(int row, int col, int value);
	double getTotalSquaredError();
	double getBlockSquaredError(pair<int, int> block);

	void loadRandomSample(int& i, int& j);
	bool loadRandomSampleBlock(int& i, int& j, int i_start, int j_start);
	
	double getBlockwiseTotalSquaredError();
	void blockWiseFactorizeTasks(int iters);

	void factorize(int);
	void blockWiseFactorize();
	
	double getStepsize();
	void updateStepsize();
	void incrementSteps();
};

#endif