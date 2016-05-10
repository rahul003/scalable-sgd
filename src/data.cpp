#include <../inc/data.h>
#include <../inc/utils.h>
#include <fstream>
#include <cstdlib>
#include <dirent.h>
using namespace std;
using namespace Eigen;
int Data::getNetflixNumRatings(string type){
	return 100498277;
	// ifstream infile(path.c_str());
	// sum = 0;
	// if(infile.is_open()){
	// 	string filename;
	// 	int size;
	// 	while(infile>>filename){
	// 		infile>>size;
	// 		// rval[filename]= size;
	// 		sum+=size;
	// 	}
	// 	infile.close();
	// } else{
	// 	perror("");
	// }
}

int Data::getMovieLensNumRatings(){
	return 10000054;
}

Data::Data(){
	num_latent_ = -1;
}


Data::Data(string type, string dir_path, int num_users, int num_movies){
	num_users_ = num_users;
	num_movies_ = num_movies;
	num_latent_ = -1;
	
	vector<T> triplets;
	if(type=="netflix"){
		DIR *dir;
		struct dirent *ent;
		
		int sum = getNetflixNumRatings(type);
		// map<string, int> meta;
		// loadNetflixMetadata(dir_path+"metadata.txt", sum);	
		triplets.reserve(sum);
		if ((dir = opendir ((dir_path+"training_set").c_str())) != NULL) {
		  while ((ent = readdir (dir)) != NULL) {
		    vector<string> filename_parts = split(ent->d_name,'.');
		    if(filename_parts.size()==2 && filename_parts[1]=="txt"){
				// printf ("%s\n", ent->d_name);
		    	readNetflixTriplets(dir_path+"training_set/"+string(ent->d_name), triplets);
		    }
		  }
		  closedir (dir);
		} else {
		  perror ("");
		}
	} else if(type=="movielens"){
		ifstream infile((dir_path+"ratings.dat").c_str());
		string temp;
		while(infile>>temp){
			int user=-1,movie=-1,rating=-1;
			int i=0,prev=0;
			while(i<temp.length()){
				if(temp[i]==':'){
					string value = temp.substr(prev,i-prev);
					if(user==-1){
						user = atoi(value.c_str());
					} else if(movie==-1){
						movie = atoi(value.c_str());
					} else if(rating==-1){
						rating = atoi(value.c_str());
						break;
					}
					prev = i+2;
					i++; //extra to ignore ::
				}
				i++;
			}
			if(user>71567 || movie>65133)
				cout<<user<<" "<<movie<<" "<<rating<<endl;
			triplets.push_back(T(user,movie,rating));
			// V_[user][movie] = rating;
		}
	}
	V_ = new Eigen::SparseMatrix<double>(num_users, num_movies);
    V_->setFromTriplets(triplets.begin(), triplets.end());
    N_ = V_->nonZeros();
    cout<<"loaded data"<<endl;
}

void Data::readNetflixTriplets(string filename, vector<T>& triplets){
	ifstream infile(filename.c_str());
	if(infile.is_open()){
		int movie_id;
		infile>>movie_id;
		string line;
		infile>>line;
		cout<<line;
		while(infile>>line){
			vector<string> parts = split(line,',');
			if(parts.size()==3){
				triplets.push_back(T(atoi(parts[0].c_str()),movie_id,atoi(parts[1].c_str())/5));
			} else{
				cout<<"skipping line "<<line<<endl;
			}
		}
		infile.close();
	} else{
		perror("");
	}
	
}

void Data::shuffleV(){
	PermutationMatrix<Dynamic,Dynamic> perm(num_users_);
	perm.setIdentity();
	std::random_shuffle(perm.indices().data(), perm.indices().data()+perm.indices().size());
	(perm.transpose() * (*V_) * perm);
	
	// Eigen::SparseMatrix<double>* shuffled = new Eigen::SparseMatrix<double>(num_users_, num_movies_);
	
	// SparseSymmetricPermutationProduct<Eigen::SparseMatrix<double, Dynamic, Dynamic>, Dynamic> x = V_->twistedBy(perm);
	// x.evalTo(*shuffled);
	// V_ = shuffled;
	// V_ = perm * V_; // permute column among cols
}
int Data::getNumUsers(){
	return num_users_;
}

int Data::getNumMovies(){
	return num_movies_;
}

int Data::getN(){
	return N_;
}

int Data::getVrows(){
	return V_->rows();
}
int Data::getVcols(){
	return V_->cols();
}

// void Data::multiplyWH(){
// 	//parallelize
// 	assert(num_latent_!=-1);
// 	for(int i=0; i<num_users_; i++){
// 		for(int j=0; j<num_movies_; j++){
// 			double sum = 0.0;
// 			for(int k=0; k<num_latent_;k++){
// 				sum+=W_[i][k]*H_[k][j];
// 			}
// 			product_[i][j] = sum;
// 		}
// 	}
// }

double Data::dotProduct(int Wi, int Hi){
	double rval=0.0;
	for(int k=0; k<num_latent_;k++){
		rval+=((W_[Wi][k])*(H_[k][Hi]));
		// cout<<W_[Wi][k]*H_[k][Hi]<<" ";
	}
	// cout<<endl;
	return rval;
}

void Data::initializeFactors(int num_latent){
	//can parallelize

	num_latent_ = num_latent;

	W_ = new double*[num_users_];
	for(int i = 0; i < num_users_; ++i)
    	W_[i] = new double[num_latent];

    H_ = new double*[num_latent];
	for(int i = 0; i < num_latent; ++i)
    	H_[i] = new double[num_movies_];
    
 //    product_ = new double*[num_users_];
	// for(int i = 0; i < num_users_; ++i)
 //    	product_[i] = new double[num_movies_];

    for(int i=0; i<num_users_;i++){
    	for(int j=0; j<num_latent; j++){
    		W_[i][j] = fRand(0.0,1.0);
    	}
    }

    for(int i=0; i<num_latent;i++){
    	for(int j=0; j<num_movies_; j++){
    		H_[i][j] = fRand(0.0,1.0);
    	}
    }

    // for(int i=0; i<num_users_;i++){
    // 	for(int j=0; j<num_movies_; j++){
    // 		product_[i][j] = 0.0;
    // 	}
    // }
}

double Data::getW(int i,int k){
	return W_[i][k];
}

double Data::getH(int k,int j){
	return H_[k][j];
}

void Data::updateW(int i, int k, double dW){
	// cout<<"W "<<i<<" "<<k<<" : "<<W_[i][k]<<endl;
	W_[i][k]+=dW;
	// cout<<"W "<<i<<" "<<k<<" : "<<W_[i][k]<<endl;
}

void Data::updateH(int k, int j, double dH){
	// cout<<"H "<<k<<" "<<j<<" : "<<H_[k][j]<<endl;
	H_[k][j]+=dH;
	// cout<<"H "<<k<<" "<<j<<" : "<<H_[k][j]<<endl;
}

void Data::printV(){
	for (int a = 0; a < V_->outerSize(); ++a){
    	for (Eigen::SparseMatrix<double>::InnerIterator it(*(V_), a); it; ++it){
    		cout<<it.value()<<" ";
    	}
    	cout<<endl;
    }	
    cout<<endl;
}

void Data::printW(){
	for(int i=0; i<num_users_;i++){
    	for(int j=0; j<num_latent_; j++){
    		cout<<W_[i][j]<<" ";
    	}
    	cout<<endl;
    }
    	cout<<endl;
}

void Data::printH(){
	for(int i=0; i<num_latent_;i++){
    	for(int j=0; j<num_movies_; j++){
    		cout<<H_[i][j]<<" ";
    	}
    	cout<<endl;
    }
    	cout<<endl;
}
// Data::Data(string file_path, int num_users, int num_movies){
// 	num_users_ = num_users;
// 	num_movies_ = num_movies;
// 	ifstream infile(file_path.c_str());
// 	string temp;
// 	if(infile.is_open()){
// 		// V_ = new double*[num_users];
// 		// for(int i = 0; i < num_users; ++i)
//   //   		V_[i] = new double[num_movies];	
// 		while(infile>>temp){
// 			int user=-1,movie=-1,rating=-1;
// 			int i=0,prev=0;
// 			while(i<temp.length()){
// 				if(temp[i]==':'){
// 					string value = temp.substr(prev,i-prev);
// 					if(user==-1){
// 						user = atoi(value.c_str());
// 					} else if(movie==-1){
// 						movie = atoi(value.c_str());
// 					} else if(rating==-1){
// 						rating = atoi(value.c_str());
// 						break;
// 					}
// 					prev = i+2;
// 					i++; //extra to ignore ::
// 				}
// 				i++;
// 			}
// 			// V_[user][movie] = rating;
// 		}

// 	} else{
// 		cout<<"Failed to open given file"<<endl;
// 	}
// }	


