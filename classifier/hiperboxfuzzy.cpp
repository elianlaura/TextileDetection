#include "hiperboxfuzzy.h"


// [Chen2006]
///
/// \brief Esta función representa la función de membresía del conjunto difuso que representa el hiperbox.
/// \param x    Vector del cual se desea analizar su pertenencia.
/// \return     Grado de pertenencia del vector al hiperbox difuso.
///
float HiperboxFuzzy::membershipFunction(vector<float> x)
{
    //return pow( getNorm(minOperator(wj, x)) / (0.01 + getNorm(wj)), 2.0 );
    //[Carpenter1992] alpha >= 0.001
    return getNorm(minOperator(wj, x)) / (0.001 + getNorm(wj));
}

///
/// \brief Función getter que retorna el punto min del hiperbox.
/// \return Vector que contiene los valores del punto min del hiperbox.
///
vector<float> HiperboxFuzzy::getMinPoint() const
{
    return minPoint;
}

///
/// \brief Función setter del punto min del hiperbox.
/// \param value Vector que contiene los valores del punto min para el hiperbox.
///
void HiperboxFuzzy::setMinPoint(const vector<float> &value)
{
    minPoint = value;
}

///
/// \brief Función getter que retorna el punto max del hiperbox.
/// \return Vector que contiene los valores del punto max del hiperbox.
///
vector<float> HiperboxFuzzy::getMaxPoint() const
{
    return maxPoint;
}

///
/// \brief Función setter del punto max del hiperbox.
/// \param value    Vector que representa el punto max para el hiperbox.
///
void HiperboxFuzzy::setMaxPoint(const vector<float> &value)
{
    maxPoint = value;
}

///
/// \brief Esta función retorna el conjunto de pesos del hiperbox con la capa de entrada.
/// \return     Vector que contiene los valores de los pesos del hiperbox con la capa de entrada.
///
vector<float> HiperboxFuzzy::getWj() const
{
    return wj;
}

///
/// \brief Esta función setea los valores de los pesos y vuelve a calcular los puntos max-min del hiperbox.
/// \param value    Vector que contiene los nuevos valores del peso del hiperbox.
///
void HiperboxFuzzy::setWj(vector<float> value)
{
    wj = value;
    int middle = value.size() / 2;
    int final = value.size();
    int i;

    minPoint.resize(middle);
    maxPoint.resize(middle);

    for(i=0; i<middle; i++)
    {
        minPoint[i] = value.at(i);
    }
    for(i=middle; i<final; i++)
    {
        maxPoint[i-middle] = 1.0 - value.at(i);
    }
}

///
/// \brief Esta función retorna el Id de la clase con que está relacionada el hiperbox.
/// \return Id de la clase con que esta relacionada el hiperbox.
///
int HiperboxFuzzy::getIdCls() const
{
    return idCls;
}

///
/// \brief Función setter del Id de la clase con que estará enlazada el hiperbox.
/// \param value    Id de la clase con que se enlazará el hiperbox.
///
void HiperboxFuzzy::setIdCls(int value)
{
    idCls = value;
}

///
/// \brief Función que se encarga de la actualización de los pesos del hiperbox.
///
void HiperboxFuzzy::updateWj()
{
    wj = minPoint;
    int n = maxPoint.size();
    for(int i=0; i<n; i++)
    {
        wj.push_back(1 - maxPoint.at(i));
    }
}

