#ifndef HELPERNETWOTK_H
#define HELPERNETWOTK_H

#include <vector>
#include <cmath>

using namespace std;

namespace {

vector<float> minOperator(vector<float> a, vector<float> b)
{
    int numElem = a.size();
    vector<float> res(numElem);

    for(int i=0; i<numElem; i++)
    {
        res[i] = min(a.at(i), b.at(i));
    }
    return res;
}

float getNorm(vector<float> x)
{
    int numElem = x.size();
    float norm = 0;

    for(int i=0; i<numElem; i++)
    {
        norm += abs(x.at(i));
    }
    return norm;
}

}

#endif // HELPERNETWOTK_H
