#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMovie>
#include <QFileInfo>
#include <QFileDialog>
#include <QPalette>
#include <QMessageBox>
#include <QTime>

#include <iostream>
#include <stdio.h>

#include "video.h"
#include "dirent.h"
/*#include "../classifier/fuzzyartmap.h"
#include "../classifier/helperNetwork.h"
#include "../classifier/hiperboxfuzzy.h"
#include "../featuresExtractor/featuresextractor.h"
#include "../featuresExtractor/shape.h"
#include "../DefectClassifier/image.h"
#include "../DefectClassifier/imagehelper.h"
#include "../DefectClassifier/imageprocessor.h"
#include "../DefectClassifier/helpers.h"
#include "../DefectClassifier/managerpatterns.h"
#include "../DefectClassifier/systemcontroller.h"
#include "../DefectClassifier/systemcore.h"*/

using namespace std;
using namespace cv;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void updateVideoImage(QImage img, QString type_defect);
    void Load();
    void Play_Stop();
    QString getFormattedTime(int timeInSeconds);
    void on_horizontalSlider_sliderPressed();
    void on_horizontalSlider_sliderReleased();
    void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::Widget *ui;
    QString currentMovieDirectory;
    //Mat img_original;
    Video* MyVideo;
};

#endif // WIDGET_H
