#ifndef SYSTEMCONTROLLER_H
#define SYSTEMCONTROLLER_H

#include <QMainWindow>
#include "systemcore.h"
#include <fstream>
#include <map>

///
/// \brief La clase SystemController se encarga de interactuar con el GUI del sistema.
///
/** Recibe las rutas de los archivos de entrenamiento, de la imagen a clasificar o la ruta donde se encuentra
 * el archivo de configuración de la red neuronal para cargarla.
*/


class SystemController
{
private:
    const char *fileConfig = "configFNN.txt";
    const char *fileClass = "clases.txt";
    map<int, QString> mapClass;
    SystemCore core;

    void loadClassData();
    QString getLabelClass(int idCls);
    void saveInfoNetwork();

public:
    SystemController();

    void trainNetwork(QString pathFile, bool saveNet=false);
    void loadNetworkFrom(QString path);

    vector<float> calculatePattern(QString path);
    vector<float> calculatePattern(QImage img);
    QString classifyImage(QImage img);
    QString classifyImage(QString path);

    SystemCore getCore() const;
    QPixmap getImageOriginal();
    QPixmap getImageBinary();
    QString getTextResults();
};

#endif // SYSTEMCONTROLLER_H
