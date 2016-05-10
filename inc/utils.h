#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <sstream>
#include <vector>

#define VNAME(x) #x
#define VDUMP(x) std::cout << #x << " " << x << std::endl

std::vector<std::vector<int> > permutations(std::vector<int> list);
std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
double fRand(double fMin, double fMax);
unsigned int rand_interval(unsigned int min, unsigned int max);
std::vector<int> range(int min, int max);

#endif