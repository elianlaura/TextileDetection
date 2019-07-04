#ifndef IMAGE_H
#define IMAGE_H


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <highgui.h>
#include <QImage>

using namespace cv;
using namespace std;

///
/// \brief La clase Image representa la imagen que será entrada del sistema y se procesará.
///
class Image : public Mat
{
public:
    Image();
    Image(int rows, int cols, int type);
    Image(Size size, int type);
    Image(int rows, int cols, int type, const Scalar &s);
    Image(Size size, int type, const Scalar &s);
    Image(const Mat &m);
    Image(const Image &m, const Rect &roi);
    //Image(const Mat *m, bool copyData = false);

    QImage convertToQimage();
    void calculateProfiles(int sizeWindow, vector<float> &Hsum, vector<float> &Vsum);
    Image getROI(int sizeWindow, vector<float> Hsum, vector<float> Vsum);
private:
    void findROI(vector<float> histogram, int pointIni, float maxVal, int numSecc, int &ini, int &fin, int sizeBorder = 3);
};

Image convertToImage( const QImage &inImage, bool inCloneImageData = true );

#endif // IMAGE_H
