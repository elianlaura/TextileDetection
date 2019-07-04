#include "fuzzyartmap.h"
#include "helperNetwork.h"

#define FAMILIAR_THRES  0.97


vector<float> FuzzyARTMAP::transformPattern(vector<float> pattern)
{
    int i;
    vector<float> result = pattern;
    int numElem = pattern.size();

    for(i=0; i<numElem; i++)
    {
        result.push_back(1.0 - pattern.at(i));
    }
    return result;
}

void FuzzyARTMAP::splitInTwoSets(int labelClass, vector<int> labels, vector<int > &posSet, vector<int > &negSet)
{
    posSet.clear();
    negSet.clear();

    int numElem = labels.size();
    for(int i=0; i<numElem; i++)
    {
        if(labels.at(i) == labelClass)
        {
            posSet.push_back(i);
        }
        else
        {
            negSet.push_back(i);
        }
    }
}

///
/// \brief Esta función inicializa la red neuronal con los datos mínimos necesarios.
/// \param sizePattern  Entero que indica el tamaño del patrón o número de descriptores que representan a un defecto.
/// \param numCls       Entero que representa el número de clases disponibles para clasificar.
///
void FuzzyARTMAP::createNetwork(int sizePattern, int numCls)
{
    sizeInput = sizePattern;
    this->numCls = numCls;
    layerOut.resize(numCls);
    hiperboxes.clear();
}

///
/// \brief Esta función carga los datos de la red neuronal a partir de un vector que contiene la información necesaria.
/// \param info     Vector que contiene la información necesaria para inicializar una red ARTMAP difusa.
///
void FuzzyARTMAP::loadNetwork(vector<vector<float> > info)
{
    createNetwork(info.at(0).at(1), info.at(0).at(0));
    HiperboxFuzzy *tmpHbox;

    for(int i=1; i<info.size(); i+=3)
    {
        tmpHbox = new HiperboxFuzzy();
        tmpHbox->setIdCls(info.at(i).at(0));
        tmpHbox->setMinPoint(info.at(i+1));
        tmpHbox->setMaxPoint(info.at(i+2));
        tmpHbox->updateWj();
        hiperboxes.push_back(tmpHbox);
        layerOut.at(info.at(i).at(0)).push_back(hiperboxes.size()-1);
    }
}

///
/// \brief Esta función devuelve un vector que contiene la información de la red neuronal,
/// como los hiperboxes y las clases con las que estan relacionados, etc.
/// \return Vector que contiene la información de la red neuronal ARTMAP difusa.
///
vector<vector<float> > FuzzyARTMAP::getInfoNetwork()
{
    HiperboxFuzzy *tmpHbox;
    vector<float> tmpData;

    vector<vector<float> > info(1);
    info.at(0).push_back(numCls);
    info.at(0).push_back(sizeInput);

    int k;
    for(int i=0; i<hiperboxes.size(); i++)
    {
        tmpHbox = hiperboxes.at(i);
        info.push_back(vector<float>(1, tmpHbox->getIdCls()));
        k = info.size() - 1;

        //info.at(k).push_back(tmpHbox->getIdCls());
        tmpData = tmpHbox->getMinPoint();
        info.at(k).insert(info.at(k).end(), tmpData.begin(), tmpData.end());
        tmpData = tmpHbox->getMaxPoint();
        info.at(k).insert(info.at(k).end(), tmpData.begin(), tmpData.end());
    }
    return info;
}

// [Chen2006]
void FuzzyARTMAP::applyChenMethod(vector<vector<float> > X, vector<int> labels, vector<vector<int> > &acumPatt)
{
    int m = X.size();
    hiperboxes.clear();
    acumPatt.clear();
    layerOut.clear();
    layerOut.resize(numCls);

    vector<float> input;
    vector<int> posSet, negSet;
    HiperboxFuzzy *tmpHiperbox;
    vector<float> wt, wtt;

    int numPos, cls, i, kl, j, p;
    float maxTmp, valMem;
    int idHboxMax, idH;

    for( cls=0; cls<numCls; cls++ )
    {
        splitInTwoSets(cls, labels, posSet, negSet);
        numPos = posSet.size();

        // Insertamos el primer hiperbox para la clase que seria el primer patron de la clase
        tmpHiperbox = new HiperboxFuzzy();
        tmpHiperbox->setWj(X[posSet.at(0)]);
        tmpHiperbox->setIdCls(cls);
        hiperboxes.push_back(tmpHiperbox);
        idH = hiperboxes.size() - 1;
        layerOut.at(cls).push_back(idH);
        acumPatt.push_back(vector<int>());
        acumPatt.at(idH).push_back(posSet.at(0));

        kl = layerOut.at(cls).size();

        maxTmp = -1;
        idHboxMax = 0;

        for(i=1; i<numPos; i++)
        {
            input = X[posSet.at(i)];
            // Evaluamos los hiperboxes de la clase l
            for( j=0; j<kl; j++ )
            {
                tmpHiperbox = hiperboxes.at(layerOut.at(cls).at(j));
                valMem = tmpHiperbox->membershipFunction(input);

                if(maxTmp < valMem)
                {
                    maxTmp = valMem;
                    idHboxMax = layerOut.at(cls).at(j);
                }
            }

            tmpHiperbox = hiperboxes.at(idHboxMax);
            wt = minOperator(tmpHiperbox->getWj(), input);

            // Analizamos los patrones que no pertenecen a la clase para evitar overlap
            for(p=0; p<negSet.size(); p++)
            {
                wtt = minOperator(wt, X[negSet.at(p)]);
                if( wtt == wt )
                {
                    tmpHiperbox = new HiperboxFuzzy();
                    tmpHiperbox->setWj(input);
                    tmpHiperbox->setIdCls(cls);
                    hiperboxes.push_back(tmpHiperbox);
                    idH = hiperboxes.size() - 1;
                    layerOut.at(cls).push_back(idH);
                    acumPatt.push_back(vector<int>());
                    acumPatt.at(idH).push_back(posSet.at(i));

                    break;
                }
            }

            if( p >= m - numPos)
            {
                tmpHiperbox->setWj(wt);
                acumPatt.at(idHboxMax).push_back(posSet.at(i));
            }
        }
    }
}

bool FuzzyARTMAP::existsHboxNoise(int sizeNoise, vector<vector<int> > acumPatt, vector<vector<float> > &X, vector<int> &labels)
{
    vector<vector<float> > newX;
    vector<int> newLabels;
    int qtyIni = X.size();

    for(int i=0; i<acumPatt.size(); i++)
    {
        //cout << "Hbox(" << i << "): " << acumPatt.at(i).size() << endl;

        if(acumPatt.at(i).size() > sizeNoise)
        {
            for(int j=0; j<acumPatt.at(i).size(); j++)
            {
                newX.push_back(X.at(acumPatt.at(i).at(j)));
                newLabels.push_back(labels.at(acumPatt.at(i).at(j)));
            }
        }
        //else
        //    cout << "Remover de: " << i << endl;
    }
    X = newX;
    labels = newLabels;
    return !(qtyIni == X.size());
}

///
/// \brief Esta función permite entrenar la red neuronal a partir de un conjunto de patrones de entrenamiento, conociendo las respectivas clases
/// a las que pertenecen.
/// \param patterns     Vector que contiene los patrones de entrenamiento.
/// \param labels       Vector que contiene las clases a las que corresponden cada patrón de entrenamiento.
/// \param sizeNoise    Entero que indica que un hiperbox debe contener una cantidad de patrones mayor que sizeNoise.
///
void FuzzyARTMAP::trainNetwork(vector<vector<float> > patterns, vector<int> labels, int sizeNoise)
{
    int m = patterns.size();
    vector<vector<float> > X(m);
    vector<vector<int> > acumPatt;

    // Transformamos los patrones de entrada al nuevo modo
    for(int i=0; i<m; i++)
    {
        X[i] = transformPattern(patterns.at(i));
    }
    applyChenMethod(X, labels, acumPatt);
    cout << "NUM-HIPERBOXES: " << getTotalHiperboxes() << endl;

    if(sizeNoise > 0)
    {
        while(existsHboxNoise(sizeNoise, acumPatt, X, labels))
        {
            applyChenMethod(X, labels, acumPatt);
            cout << "NUM-HIPERBOXES: " << getTotalHiperboxes() << endl;
        }
    }
}

///
/// \brief Esta función se encarga de comprobar si el patrón tiene familiaridad con el hiperbox ganador, usando
/// un umbral para su determinación.
/// \param pattern      Vector que contiene los descriptores del patrón que se va a analizar.
/// \param wj           Vector que representa el W del hiperbox ganador.
/// \return             Booleano, verdadero en caso sea familiar con el hiperbox ganador, caso contrario será falso.
///
bool FuzzyARTMAP::isFamiliar(vector<float> pattern, vector<float> wj)
{
    float valFamiliarity;
    valFamiliarity = getNorm(minOperator(pattern, wj)) / getNorm(wj);
    cout << "Familiaridad: " << valFamiliarity << endl;

    if(valFamiliarity > FAMILIAR_THRES)
    {
        return true;
    }
    return false;
}

///
/// \brief Esta función se encarga de determinar a qué clase pertenece el patrón de entrada.
/// \param pattern      Vector de descriptores que representa el patrón de una imagen.
/// \return             Entero que indica el Id de la clase a la que pertenece el defecto.
///
int FuzzyARTMAP::classifyPattern(vector<float> pattern)
{
    vector<float> input = transformPattern(pattern);
    float valMem, maxVal = -1;
    int idCls, winHbox;

    for( int i=0; i<hiperboxes.size(); i++ )
    {
        valMem = hiperboxes.at(i)->membershipFunction(input);
        if(maxVal < valMem)
        {
            maxVal = valMem;
            winHbox = i;
            idCls = hiperboxes.at(i)->getIdCls();
            cout << "Hiperbox(" << i << ") => Valmem: " << valMem << endl;
        }
    }
    if(isFamiliar(input, hiperboxes.at(winHbox)->getWj()))
        return idCls;
    else
        return -1;
}

///
/// \brief Esta función devuelve el número de hiperboxes que tiene la red neuronal en su capa oculta.
/// \return     Entero que representa el número de hiperboxes en la capa oculta.
///
int FuzzyARTMAP::getTotalHiperboxes()
{
    int totalNeurons = 0;
    for(int i=0; i<numCls; i++)
    {
        cout << "Clase" << i << " - NroHboxs: " << layerOut.at(i).size() << " de " << hiperboxes.size() << endl;
        totalNeurons += layerOut.at(i).size();
    }
    return totalNeurons;
}
