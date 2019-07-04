#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <iostream>
#include <algorithm>
#include "image.h"

using namespace cv;
using namespace std;

///
/// \brief La clase ImageProcessor se encarga del procesamiento de las imágenes, teniendo las fases de: preprocesamiento,
/// selección de ROI, segmentación y eliminación del ruido.
///
class ImageProcessor
{
public:
    static const int MAX_SIZE_IMG = 1024;       ///< Tamaño máximo que puede tener una imagen.

    ImageProcessor() {}
    Image imageProcessing(Image imageOriginal, int &imgWidth, int &imgHeight);
private:
    void preprocessing(Image imageSrc, Image &imageOut);
    void segmentation(Image imageSrc, Image &imageOut);
    void removeNoise(Image imageSrc, Image &imageOut);
    void selectionROI(Image imageSrc, Image &imageOut);
};

#endif // IMAGEPROCESSOR_H
