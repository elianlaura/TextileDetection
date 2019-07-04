#ifndef FUZZYARTMAP_H
#define FUZZYARTMAP_H

#include <iostream>
#include <vector>
#include "hiperboxfuzzy.h"

using namespace std;

///
/// \brief La clase FuzzyARTMAP representa a la red neuronal ARTMAP difusa que se encarga de la clasificación de los defectos.
///
class FuzzyARTMAP
{
private:
    int numCls;
    int sizeInput;
    vector<vector<int> > layerOut;
    vector<HiperboxFuzzy *> hiperboxes;

    vector<float> transformPattern(vector<float> pattern);
    void splitInTwoSets(int labelClass, vector<int> labels, vector<int> &posSet, vector<int> &negSet);
    void applyChenMethod(vector<vector<float> > X, vector<int> labels, vector<vector<int> > &acumPatt);
    bool existsHboxNoise(int sizeNoise, vector<vector<int> > acumPatt, vector<vector<float> > &X, vector<int> &labels);
    bool isFamiliar(vector<float> pattern, vector<float> wj);
public:
    FuzzyARTMAP() {}

    void createNetwork(int sizeInput, int numCls);
    void loadNetwork(vector<vector<float> > info);

    void trainNetwork(vector<vector<float> > patterns, vector<int> labels, int sizeNoise=0);
    int classifyPattern(vector<float> pattern);

    int getTotalHiperboxes();
    vector<vector<float> > getInfoNetwork();
};

#endif // FUZZYARTMAP_H
