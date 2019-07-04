#include <iostream>
#include "systemcore.h"


///
/// \brief Función encargada de la clasificación a partir de la ruta de una imagen.
/// \param pathImage    String que representa el path de la imagen que se desea clasificar.
/// \return             Entero que representa el ID del tipo de defecto al que pertenece la imagen.
///
int SystemCore::classifyImage(const char *pathImage)
{
    loadImageOriginal(pathImage);
    initProcess();
    return classifyPattern();
}

///
/// \brief Función encargada de la clasificación a partir de una imagen.
/// \param img      Representa la imagen que se analizará para clasificar.
/// \return         Entero que representa el ID del tipo de defecto al que pertenece la imagen.
///
int SystemCore::classifyImage(Image img)
{
    loadImageOriginal(img);
    initProcess();
    return classifyPattern();
}

///
/// \brief Función encargada de calcular el patrón característico de una imagen.
/// \param path     String que representa el path de la imagen de la cual se desea calcular el patrón.
///
void SystemCore::calculatePattern(const char *path)
{
    loadImageOriginal(path);
    initProcess();
}

///
/// \brief Función encargada de calcular el patrón característico de una imagen.
/// \param img      Imagen de la cual se desea calcular el patrón.
/// \return         Vector que contiene los descriptores obtenidos a partir de la imagen.
///
void SystemCore::calculatePattern(Image img)
{
    loadImageOriginal(img);
    initProcess();
}

///
/// \brief Función que se encarga de cargar la imagen que se analizará y guardarla en memoria.
/// \param pathImage        String que indica la ruta de la imagen a analizar.
///
void SystemCore::loadImageOriginal(const char *pathImage)
{
    Image tmp(imread(pathImage));
    loadImageOriginal(tmp);
}

///
/// \brief Función que se encarga de guardar la imagen que se desea analizar.
/// \param img      Imagen que se desea analizar.
///
void SystemCore::loadImageOriginal(Image img)
{
    images.clear();
    images.push_back(img);
    cout << ">>> Imagen Cargada\n";
}

///
/// \brief Esta función inicia el procesamiento de la imagen cargada, hasta la obtención de sus descriptores.
///
void SystemCore::initProcess()
{
    cout << ">>> Proceso Iniciado" << endl;

    Image imageOriginal = images[IMAGE_ORIGINAL];

    if(!imageOriginal.data) return;

    int imgWidth, imgHeight;
    Image imageBinary = imageProcessor.imageProcessing(imageOriginal, imgWidth, imgHeight);
    images.push_back(imageBinary);
    featuresExtractor.getFeaturesShape(imageBinary, imgWidth, imgHeight);

    cout << ">>> Proceso Terminado" << endl;
}

///
/// \brief Esta función se encarga del entrenamiento de la red neuronal a partir de un vector que contiene los patrones.
/// \param patterns     Vector que contiene los descriptores de las imágenes de entrenamiento, cada elemento es el conjunto de descriptores de una imagen.
/// \param labels       Vector de enteros que contiene los ids de la clase a la que pertenece cada patrón del vector patterns.
/// \param numCls       Entero que indica el número de clases que se tendrá.
///
void SystemCore::trainWithPatterns(vector<vector<float> > patterns, vector<int> labels, int numCls)
{
    fmm.createNetwork(patterns.at(0).size(), numCls);
    fmm.trainNetwork(patterns, labels, 1);
}

///
/// \brief Esta función se encarga de clasificar el patrón calculado.
/// \return     Entero que representa el ID de la clase a la que pertenece el patrón.
///
int SystemCore::classifyPattern()
{
    return fmm.classifyPattern(featuresExtractor.getValues());
}

///
/// \brief Función encargada de cargar la red neuronal.
/// \param configNet    Vector que contiene la información necesaria para crear una red neuronal FuzzyARTMAP.
///
void SystemCore::loadNetworkConfig(vector<vector<float> > configNet)
{
    fmm.loadNetwork(configNet);
}

///
/// \brief Función que devuelve una imagen.
/// \param index    Entero que representa el tipo de la imagen que se desea obtener.
/// \return         Retorna la imagen solicitada.
///
Image SystemCore::getImage(int index)
{
    if(index == IMAGE_ORIGINAL || index == IMAGE_BINARY)
    {
        return images[index];
    }
    else
        return Image();
}

///
/// \brief Esta función retorna el vector de descriptores de una imagen.
/// \return     Vector que contiene los descriptores de la imagen cargada.
///
vector<float> SystemCore::getFeatures()
{
    return featuresExtractor.getValues();
}

///
/// \brief Función encargada de devolver los nombres de los descriptores que se han calculado.
/// \return     Vector que contiene los nombres de los descriptores.
///
vector<string> SystemCore::getNamesFeatures()
{
    return featuresExtractor.getNames();
}

///
/// \brief Función que permite obtener la información de la red neuronal actual.
/// \return     Vector que contiene la información de la red neuronal.
///
vector<vector<float> > SystemCore::getInfoNetwork()
{
    return fmm.getInfoNetwork();
}

///
/// \brief Función getter de la red neuronal de esta clase.
/// \return     Red neuronal FuzzyARTMAP.
///
FuzzyARTMAP SystemCore::getFNN()
{
    return fmm;
}
