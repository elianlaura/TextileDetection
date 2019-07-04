#include "image.h"

Image::Image():Mat(){}

///
/// \brief Constructor de la clase.
/// \param rows     Entero que indica el número de filas que tendrá la imagen.
/// \param cols     Entero que indica el número de columnas de la imagen.
/// \param type     Entero que indica el tipo de imagen que será, a color, escala de grises, etc.
///
Image::Image(int rows, int cols, int type) : Mat(rows, cols, type) {}

///
/// \brief Constructor de la clase.
/// \param size     Del tipo Size que establece el tamaño de la imagen.
/// \param type     Entero que indica el tipo de imagen.
///
Image::Image(Size size, int type) : Mat(size, type){}

///
/// \brief Constructor de la clase.
/// \param rows     Entero que indica el número de filas de la imagen.
/// \param cols     Entero que indica el número de columnas de la imagen.
/// \param type     Entero que indica el tipo de la imagen.
/// \param s        Scalar con el que se iniciará la imagen.
///
Image::Image(int rows, int cols, int type, const Scalar& s) : Mat(rows, cols, type, s){}

///
/// \brief Constructor de la clase.
/// \param size     Size que establece el tamaño de la imagen.
/// \param type     Entero que indica el tipo de la imagen.
/// \param s        Scalar que indica el valor inicial de toda la imagen.
///
Image::Image(Size size, int type, const Scalar& s) : Mat(size, type, s){}

///
/// \brief Constructor de la clase.
/// \param m        Image de la cual se obtendrá el ROI.
/// \param roi      Rect que contiene el punto X, Y, ancho y alto donde se encuentra el ROI en la imagen.
///
Image::Image(const Image& m, const Rect& roi) : Mat(m, roi){}

///
/// \brief Constructor de la clase.
/// \param m        Imagen del tipo Mat a partir del cual se creará la imagen.
///
Image::Image(const Mat &m) : Mat(m){}

///
/// \brief Constructor de la clase.
/// \param m        Imagen del tipo Mat a partir del cual se creará la imagen.
/// \param copyData Booleano que indica si se copia los datos en la nueva imagen.
///
//Image::Image(const Mat* m, bool copyData) : Mat(m, copyData){}


///
/// \brief Esta función convierte la imagen en una imagen de clase QImage.
/// \return     La imagen con clase QImage.
///
QImage Image::convertToQimage()
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)this->data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, this->cols, this->rows, this->step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(this->type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)this->data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, this->cols, this->rows, this->step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        //cout << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

#include <iostream>
Image convertToImage( const QImage &inImage, bool inCloneImageData )
{
    Mat out;
  switch ( inImage.format() )
  {
     // 8-bit, 4 channel
     case QImage::Format_RGB32:
     {
        cv::Mat  mat( inImage.height(), inImage.width(), CV_8UC4, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine() );

        //return (inCloneImageData ? mat.clone() : mat);
        out = (inCloneImageData ? mat.clone() : mat);
        return Image(out);
     }

     // 8-bit, 3 channel
     case QImage::Format_RGB888:
     {
        if ( !inCloneImageData )
           std::cout << "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";

        QImage   swapped = inImage.rgbSwapped();

        //return cv::Mat( swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine() ).clone();
        out = cv::Mat( swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine() ).clone();
        return Image(out);
     }

     // 8-bit, 1 channel
     case QImage::Format_Indexed8:
     {
        cv::Mat  mat( inImage.height(), inImage.width(), CV_8UC1, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine() );

        out = (inCloneImageData ? mat.clone() : mat);
        return Image(out);
     }

     default:
        std::cout << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
        break;
  }

  return Image(Mat());
}


///
/// \brief Esta función se encarga de calcular los perfiles vertical y horizontal de la imagen (escala de grises o binaria).
/// \param sizeWindow       Entero que indica el tamaño de la ventana, número de pixeles a evaluar
/// \param Hsum             Vector donde se almacenara el perfil horizontal
/// \param Vsum             Vector donde se almacenara el perfil vertical
///
void Image::calculateProfiles(int sizeWindow, vector<float> &Hsum, vector<float> &Vsum)
{
    int numIntV = ceil(this->rows * 1.0 / sizeWindow);
    int numIntH = ceil(this->cols * 1.0 / sizeWindow);
    int intervalH, intervalV;

    Hsum.resize(numIntH, 0);
    Vsum.resize(numIntV, 0);

    for(int i=0; i<this->rows; i++)
    {
        for(int j=0; j<this->cols; j++)
        {
            intervalV = i / sizeWindow;
            intervalH = j / sizeWindow;

            Vsum[intervalV] += (float)this->at<uchar>(i,j);
            Hsum[intervalH] += (float)this->at<uchar>(i,j);
        }
    }
    Hsum[0] = 0;    Hsum[numIntH-1] = 0;
    Vsum[0] = 0;    Vsum[numIntV-1] = 0;
}



void Image::findROI(vector<float> histogram, int pointIni, float maxVal, int numSecc, int &ini, int &fin, int sizeBorder)
{
    int i=0, k=0;
    float thres = maxVal / numSecc;

    ini = pointIni;
    fin = pointIni;

    for(i=pointIni-1; i>=0; i--)
    {
        if(histogram[i] >= thres)
        {
            if((ini - i) <= 3)
                ini = i;
            else
                break;
        }
    }

    // Hallando inicio, inicio
    // Analizamos los tres laterales izquierdos del INI hallado
    k=0;
    for(i=ini-1; i>=0 && k<sizeBorder; i--)
    {
        k++;
        if(histogram[i] < histogram[ini])
        {
            ini = i;
        }
        else
            break;
    }

    for(i=pointIni+1; i<histogram.size(); i++)
    {
        if(histogram[i] >= thres )
        {
            if((i - fin) <= 3)
                fin = i;
            else
                break;
        }
    }

    // Hallando fin, fin
    // Analizamos los tres laterales derechos del FINI hallado
    k=0;
    for(i=fin+1; i<histogram.size() && k<sizeBorder; i++)
    {
        k++;
        if(histogram[i] < histogram[fin])
        {
            fin = i;
        }
        else
            break;
    }
    //cout << "--> Rango: [ " << ini << " , " << fin << " ]\n";
}

///
/// \brief Esta función se encarga de encontrar la región de interés (ROI) de una imagen en escala de grises.
/// \param sizeWindow   Entero que representa el ancho o largo que conforma un bloque que se analizó.
/// \param Hsum         Vector que contiene la distribución de colores de los bloques verticales de la imagen.
/// \param Vsum         Vector que contiene la distribución de colores de los bloques horizontales de la imagen.
/// \return
///
Image Image::getROI(int sizeWindow, vector<float> Hsum, vector<float> Vsum)
{
    Mat hmat(Hsum), vmat(Vsum);
    Point minPos, maxPosH, maxPosV;
    double minVal, maxValH, maxValV;

    minMaxLoc(hmat, &minVal, &maxValH, &minPos, &maxPosH);
    minMaxLoc(vmat, &minVal, &maxValV, &minPos, &maxPosV);

    int ini, fin;
    Rect rectRoi;

    if(maxValH > maxValV)
    {
        findROI(Hsum, maxPosH.y, maxValH, 2, ini, fin);
        rectRoi.x = ini * sizeWindow;
        rectRoi.width = std::min((fin-ini) * sizeWindow + sizeWindow, this->cols - rectRoi.x);
        rectRoi.y = 0;
        rectRoi.height = this->rows;
        //cout << "--> ROI H> x: " << rectRoi.x << " y: " << rectRoi.y << " width: " << rectRoi.width << " height: " << rectRoi.height << endl;
    }
    else
    {
        findROI(Vsum, maxPosV.y, maxValV, 2, ini, fin);
        rectRoi.x = 0;
        rectRoi.width = this->cols;
        rectRoi.y = ini * sizeWindow;
        rectRoi.height = std::min((fin-ini) * sizeWindow + sizeWindow, this->rows - rectRoi.y);
        //cout << "--> ROI V> x: " << rectRoi.x << " y: " << rectRoi.y << " width: " << rectRoi.width << " height: " << rectRoi.height << endl;
    }
    Image tmp;
    this->copyTo(tmp);
    Image roi = tmp(rectRoi);
    return roi.clone();
}

