#ifndef HIPERBOXFUZZY_H
#define HIPERBOXFUZZY_H

#include <iostream>
#include "helperNetwork.h"

class FuzzyARTMAP;

///
/// \brief La clase HiperboxFuzzy representa un subconjunto difuso de cada clase de defecto.
///
class HiperboxFuzzy
{
    friend class FuzzyARTMAP;

private:
    int idCls;
    vector<float> minPoint; // uj
    vector<float> maxPoint; // vj
    vector<float> wj;       // wj

protected:
    float membershipFunction(vector<float> x);

    vector<float> getMinPoint() const;
    void setMinPoint(const vector<float> &value);

    vector<float> getMaxPoint() const;
    void setMaxPoint(const vector<float> &value);

    vector<float> getWj() const;
    void setWj(vector<float> value);
    void updateWj();

    int getIdCls() const;
    void setIdCls(int value);

public:
    HiperboxFuzzy(){}
};

#endif // HIPERBOXFUZZY_H
