#ifndef SYSTEMCORE_H
#define SYSTEMCORE_H

#include "imageprocessor.h"
#include "featuresExtractor/featuresextractor.h"
#include "classifier/fuzzyartmap.h"
#include "image.h"

using namespace std;

///
/// \brief La clase SystemCore se encarga del proceso de clasificación del defecto.
///
/**
 * En esta clase se tienen funciones encargadas de iniciar el procesamiento de la imagen, extraer las caracteristicas
 * y finalmente clasificar el defecto encontrado en la imagen.
 */
class SystemCore
{
private:
    ImageProcessor imageProcessor;
    FeaturesExtractor featuresExtractor;
    FuzzyARTMAP fmm;
    vector<Image> images;

    void loadImageOriginal(const char *pathImage);
    void loadImageOriginal(Image img);
    void initProcess();
    int classifyPattern();

public:
    static const int IMAGE_ORIGINAL = 0;    ///< Valor que representa el tipo imagen inicial (a colores)
    static const int IMAGE_BINARY   = 1;    ///< Valor que representa el tipo imagen binaria

    SystemCore() {}
    int classifyImage(const char *pathImage);
    int classifyImage(Image img);
    void calculatePattern(const char *path);
    void calculatePattern(Image img);

    void trainWithPatterns(vector<vector<float> > patterns, vector<int> labels, int numCls);
    void loadNetworkConfig(vector<vector<float> > configNet);

    Image getImage(int index);
    vector<float> getFeatures();
    vector<string> getNamesFeatures();
    vector<vector<float> > getInfoNetwork();
    FuzzyARTMAP getFNN();
};

#endif // SYSTEMCORE_H
