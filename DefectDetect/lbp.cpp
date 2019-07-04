/**
 * \file lbp.cpp
 * \brief implementacion del cuerpo de la clase LBP
 * \author Darwin Saire Pilco
 * \date 05/2014
*/

#include "lbp.h"

LBP::LBP()
{
}
//template <typename _Tp>
void LBP::OLBP_(const Mat& src, Mat& dst) {
    dst = Mat::zeros(src.rows-2, src.cols-2, CV_8UC1);
    for(int i=1;i<src.rows-1;i++) {
        for(int j=1;j<src.cols-1;j++) {
            //_Tp center = src.at<_Tp>(i,j);
            unsigned char center = src.at<unsigned char>(i,j);
            unsigned char code = 0;
            code |= (src.at<unsigned char>(i-1,j-1) > center) << 7;
            code |= (src.at<unsigned char>(i-1,j) > center) << 6;
            code |= (src.at<unsigned char>(i-1,j+1) > center) << 5;
            code |= (src.at<unsigned char>(i,j+1) > center) << 4;
            code |= (src.at<unsigned char>(i+1,j+1) > center) << 3;
            code |= (src.at<unsigned char>(i+1,j) > center) << 2;
            code |= (src.at<unsigned char>(i+1,j-1) > center) << 1;
            code |= (src.at<unsigned char>(i,j-1) > center) << 0;
            dst.at<unsigned char>(i-1,j-1) = code;
        }
    }
}

//template <typename _Tp>
void LBP::ELBP_(const Mat& src, Mat& dst, int radius, int neighbors) {
    neighbors = max(min(neighbors,31),1); // set bounds...
    // Note: alternatively you can switch to the new OpenCV Mat_
    // type system to define an unsigned int matrix... I am probably
    // mistaken here, but I didn't see an unsigned int representation
    // in OpenCV's classic typesystem...
    dst = Mat::zeros(src.rows-2*radius, src.cols-2*radius, CV_32SC1);
    for(int n=0; n<neighbors; n++) {
        // sample points
        float x = static_cast<float>(radius) * cos(2.0*M_PI*n/static_cast<float>(neighbors));
        float y = static_cast<float>(radius) * -sin(2.0*M_PI*n/static_cast<float>(neighbors));
        // relative indices
        int fx = static_cast<int>(floor(x));
        int fy = static_cast<int>(floor(y));
        int cx = static_cast<int>(ceil(x));
        int cy = static_cast<int>(ceil(y));
        // fractional part
        float ty = y - fy;
        float tx = x - fx;
        // set interpolation weights
        float w1 = (1 - tx) * (1 - ty);
        float w2 =      tx  * (1 - ty);
        float w3 = (1 - tx) *      ty;
        float w4 =      tx  *      ty;
        // iterate through your data
        for(int i=radius; i < src.rows-radius;i++) {
            for(int j=radius;j < src.cols-radius;j++) {
                //float t = w1*src.at<_Tp>(i+fy,j+fx) + w2*src.at<_Tp>(i+fy,j+cx) + w3*src.at<_Tp>(i+cy,j+fx) + w4*src.at<_Tp>(i+cy,j+cx);
                // we are dealing with floating point precision, so add some little tolerance
                //dst.at<unsigned int>(i-radius,j-radius) += ((t > src.at<_Tp>(i,j)) && (abs(t-src.at<_Tp>(i,j)) > std::numeric_limits<float>::epsilon())) << n;

                float t = w1*src.at<unsigned char>(i+fy,j+fx) + w2*src.at<unsigned char>(i+fy,j+cx) + w3*src.at<unsigned char>(i+cy,j+fx) + w4*src.at<unsigned char>(i+cy,j+cx);
                dst.at<unsigned int>(i-radius,j-radius) += ((t > src.at<unsigned char>(i,j)) && (abs(t-src.at<unsigned char>(i,j)) > std::numeric_limits<float>::epsilon())) << n;
            }
        }
    }
}

//template <typename _Tp>
void LBP::VARLBP_(const Mat& src, Mat& dst, int radius, int neighbors) {
    if( src.empty() ) throw 2;
    if( radius == '\0' || neighbors == '\0' ) throw 3;
    max(min(neighbors,31),1); // set bounds
    dst = Mat::zeros(src.rows-2*radius, src.cols-2*radius, CV_32FC1); //! result
    // allocate some memory for temporary on-line variance calculations
    Mat _mean = Mat::zeros(src.rows, src.cols, CV_32FC1);
    Mat _delta = Mat::zeros(src.rows, src.cols, CV_32FC1);
    Mat _m2 = Mat::zeros(src.rows, src.cols, CV_32FC1);
    for(int n=0; n<neighbors; n++) {
        // sample points
        float x = static_cast<float>(radius) * cos(2.0*M_PI*n/static_cast<float>(neighbors));
        float y = static_cast<float>(radius) * -sin(2.0*M_PI*n/static_cast<float>(neighbors));
        // relative indices
        int fx = static_cast<int>(floor(x));
        int fy = static_cast<int>(floor(y));
        int cx = static_cast<int>(ceil(x));
        int cy = static_cast<int>(ceil(y));
        // fractional part
        float ty = y - fy;
        float tx = x - fx;
        // set interpolation weights
        float w1 = (1 - tx) * (1 - ty);
        float w2 =      tx  * (1 - ty);
        float w3 = (1 - tx) *      ty;
        float w4 =      tx  *      ty;
        // iterate through your data
        for(int i=radius; i < src.rows-radius;i++) {
            for(int j=radius;j < src.cols-radius;j++) {
                //float t = w1*src.at<_Tp>(i+fy,j+fx) + w2*src.at<_Tp>(i+fy,j+cx) + w3*src.at<_Tp>(i+cy,j+fx) + w4*src.at<_Tp>(i+cy,j+cx);
                float t = w1*src.at<unsigned char>(i+fy,j+fx) + w2*src.at<unsigned char>(i+fy,j+cx) + w3*src.at<unsigned char>(i+cy,j+fx) + w4*src.at<unsigned char>(i+cy,j+cx);
                _delta.at<float>(i,j) = t - _mean.at<float>(i,j);
                _mean.at<float>(i,j) = (_mean.at<float>(i,j) + (_delta.at<float>(i,j) / (1.0*(n+1)))); // i am a bit paranoid
                _m2.at<float>(i,j) = _m2.at<float>(i,j) + _delta.at<float>(i,j) * (t - _mean.at<float>(i,j));
            }
        }
    }
    // calculate result
    for(int i = radius; i < src.rows-radius; i++) {
        for(int j = radius; j < src.cols-radius; j++) {
            dst.at<float>(i-radius, j-radius) = _m2.at<float>(i,j) / (1.0*(neighbors-1));
        }
    }
}

Mat LBP::OLBP(const Mat& src)
{
    Mat dst;
    OLBP_(src, dst);
    return dst;
}

Mat LBP::ELBP(const Mat& src, int radius, int neighbors)
{
    Mat dst;
    ELBP_(src, dst, radius, neighbors);
    return dst;
}

Mat LBP::VARLBP(const Mat& src, int radius, int neighbors)
{
    Mat dst;
    if( src.empty() ) throw 2;
    if( radius == '\0' || neighbors == '\0' ) throw 3;
    VARLBP_(src, dst, radius, neighbors);
    if( dst.empty() ) throw 2;
    return dst;
}
