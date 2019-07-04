#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    MyVideo = new Video();
    myModel = new MyModel(0);
    ui->setupUi(this);

    QObject::connect(MyVideo,SIGNAL(processedImage(QImage,QString)),this,SLOT(updateVideoImage(QImage,QString)) );

    ui->button_play_stop->setEnabled(false);
    ui->horizontalSlider->setEnabled(false);

    ui->tableView->setEnabled(true);

    connect( ui->button_load, SIGNAL( clicked() ), this, SLOT( Load() ) );
    connect( ui->button_play_stop, SIGNAL( clicked() ), this, SLOT( Play_Stop() ) );


    MyVideo->setSize( ui->label->height(), ui->label->width() );

    // Table
    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void Widget::Load(){


    ui->button_play_stop->setText(tr("Play"));

    currentMovieDirectory = QFileDialog::getOpenFileName(this,
                                          tr("Abrir Video"), ".",
                                          tr("Video Files (*.avi *.mpg *.mp4 *.MOV)"));
    QFileInfo name = currentMovieDirectory;

     if( !currentMovieDirectory.isEmpty() ){
        if (!MyVideo->loadVideo(currentMovieDirectory.toAscii().data()))
        {
            QMessageBox msgBox;
            msgBox.setText("El video seleccionado no se pudo abrir!");
            msgBox.exec();
        }
        else{
            string name_file_string = name.fileName().toStdString();
            string name_file = name_file_string.substr( 0, name_file_string.length() - 3 ) + "txt";
            cout << name_file << endl;
            this->setWindowTitle(name.fileName());
            ui->button_play_stop->setEnabled(true);
            ui->horizontalSlider->setEnabled(true);
            ui->horizontalSlider->setMaximum(MyVideo->getNumberOfFrames());
            ui->label_3->setText( getFormattedTime( (int)MyVideo->getNumberOfFrames()/(int)MyVideo->getFrameRate()) );
        }
    }

    /*DIR *dir;
    const char * directorioClase2;
    string ruta_img;
    struct dirent *fichero;
    Mat img_ori;
    string directorio = "imgs/";
    for(int c= 0; c<1; c++ ){
         directorioClase2 = directorio.c_str();
         dir=opendir(directorioClase2);
         if (dir!=NULL) {
             int radio = 20;
             int vecinos = 15;
             int bloques = 30;

             Mat RGBframe;
             Mat img_entropia;
             Mat img_correlacion;
             Mat img_energia;
             Mat img_contraste;
             Mat img_homogeneidad;
             Mat img_union;
             int scale = 16;
             Mat img_current;
             Mat img_original2;

             while (fichero=readdir(dir)){
                 if(fichero->d_type != DT_DIR){
                     std::string name(fichero->d_name);
                     //if(hasEnding(name,"JPG") || hasEnding(name,"jpg")){
                     cout<<name<<endl;
                     ruta_img = directorio +  name;
                     img_ori = imread(ruta_img);
                     MyVideo->procesaImgs(img_ori, name);

                     //}
                 }
             }
         }
    }*/


}

void Widget::updateVideoImage( QImage img, QString type_defect ){
    if (!img.isNull()){
        ui->label->setAlignment(Qt::AlignCenter);
        ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
        ui->horizontalSlider->setValue(MyVideo->getCurrentFrame());
        ui->label_2->setText( getFormattedTime( (int)MyVideo->getCurrentFrame()/(int)MyVideo->getFrameRate()) );
        ui->label_tipo_defecto->setText(type_defect);

        //ui->tableView->setModel(myModel);
    }
}
void Widget::initializeModel(QSqlTableModel *model)
{
    model->setTable("person");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("First name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Last name"));
}

void Widget::Play_Stop(){
    if(MyVideo->isStopped())
    {
        MyVideo->Play();
        ui->button_play_stop->setText( tr( "Stop" ) );
        ui->button_load->setEnabled( false );
    }else
    {
        MyVideo->Stop();
        ui->button_play_stop->setText( tr( "Play" ) );
        ui->button_load->setEnabled( true );
    }
}



QString Widget::getFormattedTime(int timeInSeconds){

    int seconds = (int) ( timeInSeconds ) % 60 ;
    int minutes = (int) ( ( timeInSeconds / 60 ) % 60 );
    int hours   = (int) ( ( timeInSeconds / ( 60 * 60 ) ) % 24 );

    QTime t(hours, minutes, seconds);
    if (hours == 0 )
        return t.toString("mm:ss");
    else
        return t.toString("h:mm:ss");
}

void Widget::on_horizontalSlider_sliderPressed()
{
    MyVideo->Stop();
}

void Widget::on_horizontalSlider_sliderReleased()
{
    MyVideo->Play();
}

void Widget::on_horizontalSlider_sliderMoved(int position)
{
    MyVideo->setCurrentFrame(position);
    ui->label_2->setText( getFormattedTime( position/(int)MyVideo->getFrameRate()) );
}

Widget::~Widget()
{
    delete ui;
}
