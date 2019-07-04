
#include "video.h"

Video::Video(QObject *parent)
 : QThread(parent)
{
    stop = true;
    this->scale = 16;
    this->radio = 13;//18;//20;
    this->vecinos = 4;//13;//15;
    this->bloques = 15; //20
}

bool Video::loadVideo(string filename) {

    capture  =  new cv::VideoCapture(filename);

    if (capture->isOpened())
    {
        frameRate = 7;
      capture->set(CV_CAP_PROP_POS_MSEC, frameRate);
        //capture->set(CV_CAP_PROP_FRAME_WIDTH,1024);
        //frameRate = (int) capture->get(CV_CAP_PROP_FPS);
        return true;
    }
    else
        return false;
}

void Video::Play()
{
    if (!isRunning()) {
        if (isStopped()){
            stop = false;
        }
        start(LowPriority);
    }
}

void Video::setSize( int height, int width ){
    this->box_h = height;
    this->box_c = width;
}

void Video::procesaImgs( Mat img, string name){

        RGBframe = img;

        Mat img_gray, img_lbp;
        cvtColor( RGBframe, img_gray, CV_BGR2GRAY );

}

void Video::run()
{
    int delay = 0; //(1000/frameRate);
    bool detect_defect = false;
    QString type_defect="";
    //cout << "delay: " << delay << "  frameRate: " << frameRate << endl;
    while(!stop){
        if (!capture->read(frame))
        {
            stop = true;
            break;
        }
        else{
            if (frame.channels()== 3){

                ////////////////////////////////////////////////////////
                float h = box_h / ( frame.size().height * 1.0 );
                float c = box_c / ( frame.size().width * 1.0 );



                Mat img_entropia;
                Mat img_correlacion;
                Mat img_energia;
                Mat img_contraste;
                Mat img_homogeneidad;
                Mat img_union;
                int scale = 16;
                Mat img_current;
                Mat img_original2 = RGBframe;
                //imshow("GLCM", img_current);


                RGBframe  = img_union;
                //img = QImage((const unsigned char*)(RGBframe.data), RGBframe.size().width, RGBframe.size().height,RGBframe.step, QImage::Format_Indexed8 ).rgbSwapped();

                ///////////////////////////////////////////////////////
                img = QImage((const unsigned char*)(this->RGBframe.data), this->RGBframe.cols, this->RGBframe.rows, this->RGBframe.step, QImage::Format_RGB888).rgbSwapped();
            }
            else
            {
                img = QImage((const unsigned char*)(frame.data), frame.size().width, frame.size().height,frame.step, QImage::Format_Indexed8 ).rgbSwapped();
            }
            emit processedImage(img,type_defect);
            this->msleep(delay);
        }
    }
}

Video::~Video()
{
    mutex.lock();
    stop = true;
    capture->release();
    delete capture;
    condition.wakeOne();
    mutex.unlock();
    wait();
}
void Video::Stop()
{
    stop = true;
}
void Video::msleep(int ms){
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}
bool Video::isStopped() const{
    return this->stop;
}

double Video::getCurrentFrame(){

    return capture->get(CV_CAP_PROP_POS_FRAMES);
}

double Video::getNumberOfFrames(){

    return capture->get(CV_CAP_PROP_FRAME_COUNT);
}

double Video::getFrameRate(){
    return frameRate;
}

void Video::setCurrentFrame( int frameNumber )
{
    capture->set(CV_CAP_PROP_POS_FRAMES, frameNumber);
}

/*void Video::endFrame(){
    emit endReproduce();
}*/
