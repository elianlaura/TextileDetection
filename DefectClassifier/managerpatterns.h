#ifndef MANAGERPATTERNS_H
#define MANAGERPATTERNS_H

#include <vector>
#include <string>

using namespace std;

///
/// \brief La clase ManagerPatterns se encarga del manejo de los patrones, como almacenarlos en archivos o obtenerlos desde algún archivo.
///
class ManagerPatterns
{
public:
    ManagerPatterns(){}
    static void savePatternsCSV(string nameFile, vector<vector<float> > patterns, vector<int> labels);
    static void savePatternsARFF(string nameFile, vector<vector<float> > patterns, vector<int> labels);
    static void getPatternsFromFile(string pathFile, vector<vector<float> > &patterns, vector<int> &labels);
};

#endif // MANAGERPATTERNS_H
