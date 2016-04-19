#include <../inc/data.h>
#include <../inc/utils.h>
#include <fstream>
#include <cstdlib>
#include <dirent.h>
using namespace std;

void Data::loadNetflixMetadata(string path, int& sum, map<string, int>& rval){
	ifstream infile(path.c_str());
	sum = 0;
	if(infile.is_open()){
		string filename;
		int size;
		while(infile>>filename){
			infile>>size;
			rval[filename]= size;
			sum+=size;
		}
		infile.close();
	} else{
		perror("");
	}
}

Data::Data(){

}

Data::Data(string dir_path, int num_users, int num_movies){
	num_users_ = num_users;
	num_movies_ = num_movies;
	DIR *dir;
	struct dirent *ent;
	int sum;
	map<string, int> meta;
	loadNetflixMetadata(dir_path+"metadata.txt", sum, meta);	
	vector<T> triplets;
	triplets.reserve(sum);
	if ((dir = opendir ((dir_path+"training_set").c_str())) != NULL) {
	  while ((ent = readdir (dir)) != NULL) {
	    vector<string> filename_parts = split(ent->d_name,'.');
	    if(filename_parts.size()==2 && filename_parts[1]=="txt"){
			printf ("%s\n", ent->d_name);
	    	readAsTriplets(dir_path+"training_set/"+string(ent->d_name), triplets);
	    }

	    //debug purposes
	    if(triplets.size()>100000)
	    	break;

	  }
	  closedir (dir);
	} else {
	  perror ("");
	}

	V_ = Eigen::SparseMatrix<double>(num_users, num_movies);
    V_.setFromTriplets(triplets.begin(), triplets.end());
    cout<<"loaded data"<<endl;
}

void Data::readAsTriplets(string filename, vector<T>& triplets){
	ifstream infile(filename.c_str());
	if(infile.is_open()){
		int movie_id;
		infile>>movie_id;
		string line;
		infile>>line;
		while(infile>>line){
			vector<string> parts = split(line,',');
			if(parts.size()==3){
				triplets.push_back(T(atoi(parts[0].c_str()),movie_id,atoi(parts[1].c_str())));
			} else{
				cout<<"skipping line "<<line<<endl;
			}
		}
		infile.close();
	} else{
		perror("");
	}
	
}

int Data::getNumUsers(){
	return num_users_;
}

int Data::getNumMovies(){
	return num_movies_;
}

void Data::initializeFactors(int num_latent){
	//can parallelize
	W_.setRandom(num_users_,num_latent);
	H_.setRandom(num_latent, num_movies_);
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


