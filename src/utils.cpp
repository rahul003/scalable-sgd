#include <../inc/utils.h>
#include <cstdlib>
#include <algorithm>
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

// [min,max)
std::vector<int> range(int min, int max){
    std::vector<int> rval;
    for(int i=min; i<max; i++){
        rval.push_back(i);
    }
    return rval;
}

std::vector<std::vector<int> > permutations(std::vector<int> list){
    std::vector<std::vector<int> > rval;
    do {
        std::vector<int> v;
        for(int i=0; i<list.size(); i++){
            v.push_back(list[i]);
        }
        rval.push_back(v);
    } while (next_permutation(list.begin(), list.end()));
    return rval;
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

unsigned int rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do
    {
        r = rand();
    } while (r >= limit);
    return min + (r / buckets);
}