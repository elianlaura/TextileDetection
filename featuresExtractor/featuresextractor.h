#ifndef FEATURESEXTRACTOR_H
#define FEATURESEXTRACTOR_H

#include <iostream>
#include "shape.h"
#include "../DefectClassifier/image.h"

using namespace std;
using namespace cv;

///
/// \brief La clase FeaturesExtractor se encarga de la extracción de características a partir de una imagen binaria
///
class FeaturesExtractor
{
private:
    vector<string> names;
    vector<float> values;

    void getContourGeneral(vector<Point> points, vector<Point> &contourGeneral);
    bool getShape(Image image, Shape &shape);

public:
    FeaturesExtractor() {}
    void getFeaturesShape(Image image, int imgWidth, int imgHeight);
    vector<string> getNames() const;
    vector<float> getValues() const;
};

#endif // FEATURESEXTRACTOR_H
