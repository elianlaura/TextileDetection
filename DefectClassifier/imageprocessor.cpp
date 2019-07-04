#include "imageprocessor.h"
#include "imagehelper.h"


bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 )
{
    double i = fabs( cv::contourArea(cv::Mat(contour1)) );
    double j = fabs( cv::contourArea(cv::Mat(contour2)) );
    //return ( i < j );
    return ( i > j );
}

///
/// \brief Función encargada de iniciar el procesamiento de la imagen.
/// \param imageOriginal    Imagen de entrada a la cual se le aplicará el procesamiento.
/// \param imgWidth         Variable donde se almacenará el ancho final de la imagen.
/// \param imgHeight        Variable donde se almacenará el alto final de la imagen.
/// \return     Imagen procesada, segmentada y binarizada después de todo el procesamiento.
///
Image ImageProcessor::imageProcessing(Image imageOriginal, int &imgWidth, int &imgHeight)
{
    // Resize la imagen si es muy grande, conservando sus proporciones
    cout << "ImageSizeInicial: [ " << imageOriginal.cols << " , " << imageOriginal.rows << " ]" << endl;

    if(imageOriginal.rows > MAX_SIZE_IMG || imageOriginal.cols > MAX_SIZE_IMG)
    {
        Image imgTemp;

        if(imageOriginal.rows > imageOriginal.cols) //Mas alto que ancho
        {
            imgHeight = MAX_SIZE_IMG;
            imgWidth = (imageOriginal.cols * 1.0 / imageOriginal.rows) * imgHeight;
        }
        else
        {
            imgWidth = MAX_SIZE_IMG;
            imgHeight = (imageOriginal.rows * 1.0 / imageOriginal.cols) * imgWidth;
        }
        resize(imageOriginal, imgTemp, Size(imgWidth, imgHeight));
        imageOriginal = imgTemp;
    }
    else
    {
        imgWidth = imageOriginal.cols;
        imgHeight = imageOriginal.rows;
    }
    cout << "ImageSizeFinal: [ " << imageOriginal.cols << " , " << imageOriginal.rows << " ]" << endl;


    Image src, output;
    Scalar avgI = mean(imageOriginal);

    //blur(imageOriginal, imageOriginal, Size(3,3));
    //ImageHelper::showImageInWindow("ImagenOriginal", imageOriginal);

    // Si la imagen es muy clara, debe tener fondo negro, por eso lo invertimos para que el fondo sea blanco
    if(avgI[0] > 139 && avgI[1] > 139 && avgI[2] > 139)
        bitwise_not(imageOriginal, src);
    else
        src = imageOriginal.clone();

    preprocessing(src, output);
    selectionROI(output, output);
    segmentation(output, output);
    return output;
}

void ImageProcessor::preprocessing(Image imageSrc, Image &imageOut)
{
    Scalar avgI, stdDesvI;
    vector<Mat> vrgb;
    int idxMin;

    Mat tmp = imageSrc - Scalar(128, 128, 128);

    //ImageHelper::showImageInWindow("Sustraccion", tmp);
    meanStdDev(tmp, avgI, stdDesvI);
    split(tmp, vrgb);

    if(avgI[0] < avgI[1])
    {
        if(avgI[0] < avgI[2])
            idxMin = 0;
        else
        {
            if(avgI[0] == avgI[2] && stdDesvI[0] > stdDesvI[2])
                idxMin = 0;
            else
                idxMin = 2;
        }
    }
    else
    {
        if(avgI[1] < avgI[2])
            idxMin = 1;
        else
        {
            if(avgI[1] == avgI[2] && stdDesvI[1] > stdDesvI[2])
                idxMin = 1;
            else
                idxMin = 2;
        }
    }

    imageOut = Image(vrgb[idxMin]);
    //ImageHelper::showImageInWindow("ImageGris", imageOut);
}

void ImageProcessor::selectionROI(Image imageSrc, Image &imageOut)
{
    vector<float> Hsum, Vsum;
    int sizeWindow = 16;
    //ImageHelper::showImageInWindow("ImageSRC", imageSrc);
    imageSrc.calculateProfiles(sizeWindow, Hsum, Vsum);
    imageOut = imageSrc.getROI(sizeWindow, Hsum, Vsum);
    //ImageHelper::showImageInWindow("imageROI", imageOut);
}

void ImageProcessor::segmentation(Image imageSrc, Image &imageOut)
{
    Mat tmp;
    threshold(imageSrc, tmp, 0, 255, THRESH_OTSU | THRESH_BINARY);
    //ImageHelper::showImageInWindow("imageSegmented", tmp);
    removeNoise(tmp, imageOut);
    //ImageHelper::showImageInWindow("imageWithoutNoise", imageOut);
}

void ImageProcessor::removeNoise(Image imageSrc, Image &imageOut)
{
    Mat tempImg = imageSrc.clone();
    Mat outTmp = Mat::zeros( tempImg.size(), CV_8UC1 );
    vector<vector<Point> > contours;
    findContours(tempImg, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

    if(contours.size() == 0) return;

    sort(contours.begin(), contours.end(), compareContourAreas);

    double minArea;
    float factor, currArea, areaBase=1;

    for(int i=contours.size()-1; i>=0; i--)
    {
        minArea = contourArea(contours[i]);
        if(minArea > 1)
        {
            minArea = std::min(minArea, 5.0);
            break;
        }
    }

    if(contours.size() > 1)
    {
        areaBase = 0.3 * contourArea(contours[0]) + 0.7 * contourArea(contours[1]);
    }

    factor = minArea / areaBase;
    cout << ">> FACTOR: " << factor << endl;

    for( int i = 0; i<contours.size(); i++ )
    {
        currArea = contourArea(contours[i]);
        //cout << "CurrRang(" << i << "): " << (currArea * factor) << endl;

        if(currArea * factor > 0.34)
            drawContours( outTmp, contours, i, Scalar(255,255,255), CV_FILLED, 8, vector<Vec4i>(), 0, Point() );
        else
            break;
    }
    imageOut = Image(outTmp);
}
