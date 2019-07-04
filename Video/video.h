#ifndef VIDEO_H
#define VIDEO_H

#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include "../DefectClassifier/systemcontroller.h"

using namespace std;
using namespace cv;
class Video : public QThread
{    Q_OBJECT
 private:
    bool stop;
    QMutex mutex;
    QWaitCondition condition;
    Mat frame;
    int frameRate;
    VideoCapture *capture;
    Mat RGBframe;
    QImage img;
    SystemController controller;

 signals:
     void processedImage(const QImage &image, QString type_defect );

 protected:
     void run();
     void msleep(int ms);

 public:
    Video(QObject *parent = 0);
    ~Video();
    bool loadVideo(string filename);
    void Play();
    void Stop();
    bool isStopped() const;
    void setCurrentFrame( int frameNumber);
    double getFrameRate();
    double getCurrentFrame();
    double getNumberOfFrames();
    void procesaImgs( Mat img, string name);
    void setSize( int height, int width );

  private:
     int box_h, box_c;
     int scale;
     int radio;
     int vecinos;
     int bloques;
     double ***Descriptors;
     double **Matrix_GLCM;
};

#endif // VIDEO_H
