#include "imagehelper.h"


///
/// \brief Muestra la imagen en una ventana individual.
/// \param nameWindow       String que representa el nombre que tendrá la ventana en su barra de título.
/// \param image            Imagen que se va a mostrar en la ventana.
/// \return                 0
///
int ImageHelper::showImageInWindow(char *nameWindow, Mat image)
{
    namedWindow(nameWindow, CV_WINDOW_NORMAL);
    imshow(nameWindow, image);
    return 0;
}

///
/// \brief Se encarga de graficar los contornos, bordes, centro de masa, entre otros, del defecto en una imagen binaria.
/// \param src              Imagen de origen.
/// \param dst              Imagen donde se guardará el resultado.
/// \param centroMasa       Punto que representa el centro de masa.
/// \param rectGeneral      Rectangulo que encierra el defecto.
/// \param contourGeneral   Vector de puntos del contorno general del defecto.
/// \param contours         Vector que contiene los diferentes contornos de las regiones defectuosas que conforman el defecto.
///
void ImageHelper::drawImageContours(Mat src, Mat &dst, Point centerMass, Rect rectGeneral, vector<Point> contourShape, vector<vector<Point> > contours)
{
    vector<vector<Point> > contourGeneral(1);
    contourGeneral[0] = contourShape;

    int numReg = contours.size();
    RNG rng(12345);
    Scalar color;
    dst = Mat::zeros( src.size(), CV_8UC3 );

    for( int i = 0; i < numReg; i++ )
    {
        color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( dst, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    }

    drawContours(dst, contourGeneral, 0, Scalar(0, 255, 255));

    if(numReg > 0)
    {
        circle(dst, centerMass, 2, Scalar(255,255,255), -1);
        rectangle( dst, rectGeneral.tl(), rectGeneral.br(), color, 2, 8, 0 );
    }
}

