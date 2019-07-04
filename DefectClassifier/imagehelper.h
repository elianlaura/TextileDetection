#ifndef IMAGEHELPER_H
#define IMAGEHELPER_H

#include <iostream>
#include <QImage>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

///
/// \brief La clase ImageHelper es una clase de ayuda con imágenes, que permite mostrar una imagen en una ventana individual o graficar ciertos componentes
/// de la imagen como su centro de masa, contornos, etc.
///
class ImageHelper
{
public:
    static int showImageInWindow(char *nameWindow, Mat image);
    static void drawImageContours(Mat src, Mat &dst, Point centerMass, Rect rectGeneral, vector<Point> contourShape, vector<vector<Point> > contours);
};

#endif // IMAGEHELPER_H
