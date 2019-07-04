#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "imagehelper.h"

using namespace cv;
using namespace std;

namespace {

bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 )
{
    double i = fabs( cv::contourArea(cv::Mat(contour1)) );
    double j = fabs( cv::contourArea(cv::Mat(contour2)) );
    //return ( i < j );
    return ( i > j );
}


void drawHistogram(char *nameHist, Mat hist, int histSize)
{
    int hist_w = 512; int hist_h = 400;
    //int histSize = 256;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    Mat b_hist = hist.clone();

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
    }

    ImageHelper::showImageInWindow(nameHist, histImage);
}
/*
void findROI(vector<float> histogram, int pointIni, float maxVal, int numSecc, int &ini, int &fin, int sizeBorder=3)
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
    cout << "--> Rango: [ " << ini << " , " << fin << " ]\n";
}

Mat getROI(Mat src, vector<float> Hsum, vector<float> Vsum, int sizeWindow)
{
    Mat hmat(Hsum), vmat(Vsum);
    Point minPos, maxPosH, maxPosV;
    double minVal, maxValH, maxValV;

    minMaxLoc(hmat, &minVal, &maxValH, &minPos, &maxPosH);
    minMaxLoc(vmat, &minVal, &maxValV, &minPos, &maxPosV);

    int ini, fin;
    Rect rectRoi;
    Mat roi;

    if(maxValH > maxValV)
    {
        findROI(Hsum, maxPosH.y, maxValH, 2, ini, fin);
        rectRoi.x = ini * sizeWindow;
        rectRoi.width = std::min((fin-ini) * sizeWindow + sizeWindow, src.cols - rectRoi.x);
        rectRoi.y = 0;
        rectRoi.height = src.rows;
        cout << "--> ROI H> x: " << rectRoi.x << " y: " << rectRoi.y << " width: " << rectRoi.width << " height: " << rectRoi.height << endl;
    }
    else
    {
        findROI(Vsum, maxPosV.y, maxValV, 2, ini, fin);
        rectRoi.x = 0;
        rectRoi.width = src.cols;
        rectRoi.y = ini * sizeWindow;
        rectRoi.height = std::min((fin-ini) * sizeWindow + sizeWindow, src.rows - rectRoi.y);
        cout << "--> ROI V> x: " << rectRoi.x << " y: " << rectRoi.y << " width: " << rectRoi.width << " height: " << rectRoi.height << endl;
    }
    roi = src(rectRoi);
    return roi.clone();
}
*/


}


#endif // HELPERS_H
