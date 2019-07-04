#include "managerpatterns.h"
#include <fstream>
#include <iostream>
#include <qstringlist.h>

///
/// \brief Función encargada de guardar los patrones en un archivo de extensión .csv.
/// \param nameFile     String que indica el nombre que tendrá el archivo.
/// \param patterns     Vector que contiene los patrones que se desean almacenar.
/// \param labels       Vector que contiene los ids de las clases a las que pertenecen los patrones.
///
void ManagerPatterns::savePatternsCSV(string nameFile, vector<vector<float> > patterns, vector<int> labels)
{
    QString filename = "../../" + QString::fromStdString(nameFile) + ".csv";
    ofstream file(filename.toStdString().c_str(), ofstream::out);

    if( !file )
    {
        cout << "No se pudo abrir el archivo" << endl;
        return;
    }

    int nroPatterns = patterns.size();
    QString lineText = "";
    for(int i=0; i<nroPatterns; i++)
    {
        lineText = QString::number(labels.at(i)) + ",";
        for(int j=0; j<patterns.at(i).size(); j++)
        {
            lineText += "," + QString::number(patterns.at(i).at(j));
        }
        file << lineText.toStdString().c_str() << endl;
    }
    file.close();
}

///
/// \brief Función encargada de almacenar los patrones en un archivo de extensión .arff para usarse en Weka si se desea.
/// \param nameFile     String que indica el nombre que tendrá el archivo.
/// \param patterns     Vector que contiene los patrones que se desean almacenar.
/// \param labels       Vector que contiene los ids de las clases a las que pertenecen cada uno de los patrones.
///
void ManagerPatterns::savePatternsARFF(string nameFile, vector<vector<float> > patterns, vector<int> labels)
{
    QString filename = "../../" + QString::fromStdString(nameFile) + ".arff";
    ofstream file(filename.toStdString().c_str(), ofstream::out);

    if( !file )
    {
        cout << "No se pudo abrir el archivo" << endl;
        return;
    }

    file << "@RELATION defectos" << endl;
    file << "@ATTRIBUTE nroregions REAL\n" <<
            "@ATTRIBUTE excentricidad REAL\n" <<
            "@ATTRIBUTE circularidad REAL\n" <<
            "@ATTRIBUTE solidez REAL\n" <<
            "@ATTRIBUTE ratioimage REAL\n" <<
            "@ATTRIBUTE class REAL\n";
    file << "@DATA" << endl;

    int nroPatterns = patterns.size();
    QString lineText = "";
    for(int i=0; i<nroPatterns; i++)
    {
        lineText = "";
        for(int j=0; j<patterns.at(i).size(); j++)
        {
            lineText += QString::number(patterns.at(i).at(j)) + ",";
        }
        lineText += QString::number(labels.at(i));
        file << lineText.toStdString().c_str() << endl;
    }
    file.close();
}

///
/// \brief Esta función se encarga de cargar los patrones desde un archivo de extensión .csv o .txt
/// \param pathFile     String que representa el archivo desde se van a cargar los patrones.
/// \param patterns     Vector donde se almacenarán los patrones obtenidos.
/// \param labels       Vector donde se almacenarán los ids de las clases de los patrones.
///
void ManagerPatterns::getPatternsFromFile(string pathFile, vector<vector<float> > &patterns, vector<int> &labels)
{
    ifstream file(pathFile.c_str());
    string line;
    QString linePat;
    QStringList values;
    int currPos = 0;

    if( file.is_open() )
    {
        while( getline(file, line) )
        {
            linePat = QString::fromStdString(line);
            values = linePat.split(",");
            patterns.push_back(vector<float>(values.size() - 2));
            labels.push_back(values.at(0).toInt());

            for(int i=2; i<values.size(); i++)
            {
                patterns[currPos][i-2] = values.at(i).toFloat();
            }
            currPos++;
        }
        file.close();
    }
}
