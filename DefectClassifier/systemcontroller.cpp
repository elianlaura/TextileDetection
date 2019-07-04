#include <string>
#include "systemcontroller.h"
#include "managerpatterns.h"


SystemController::SystemController()
{
    loadClassData();
    loadNetworkFrom(QString(fileConfig));
}

///
/// \brief Esta función se encarga de cargar el archivo de las clases para enlazar el ID de una clase con su nombre respectivo.
/// El nombre del archivo es clases.txt
///
void SystemController::loadClassData()
{
    ifstream file(fileClass);
    string line;
    QString linePat;
    QStringList values;
    int idCls;

    if( file.is_open() )
    {
        while( getline(file, line) )
        {
            linePat = QString::fromStdString(line);
            values = linePat.split(",");
            idCls = values.at(0).toInt();
            //mapClass[idCls] = values.at(1).toStdString();
            mapClass[idCls] = values.at(1);
        }
    }
    mapClass[-1] = "Desconocido";
    file.close();
}

///
/// \brief Esta función asigna el nombre de la clase del id que se le pasa como parámetro
/// \param idCls    Entero que representa el id de alguna clase de defecto
/// \return         Retorna un string que es el nombre correspondiente para esa clase
///
QString SystemController::getLabelClass(int idCls)
{
    string cad = mapClass[idCls].toStdString();
    cout << "CLASS(" << idCls << ") => " << cad << endl;
    return (mapClass[idCls]);
}

///
/// \brief Esta función guarda la información de la red neuronal actual en un archivo llamado configFNN.txt
///
void SystemController::saveInfoNetwork()
{
    ofstream file(fileConfig, ofstream::out);
    vector<vector<float> > info = core.getInfoNetwork();

    int numCls = info.at(0).at(0);
    int sizePatt = info.at(0).at(1);

    file << numCls << endl;
    file << sizePatt << endl;

    int cls,num;
    QString line;
    for(int i=1; i<info.size(); i++)
    {
        cls = info.at(i).at(0);
        //line = "Class_" + QString::number(cls);
        line = QString::number(cls);
        file << line.toStdString() << endl;

        line = "";
        num = 0;
        for(int j=1; j<info.at(i).size(); j++)
        {
            line += QString::number(info.at(i).at(j)) + ",";
            num++;
            if(num % sizePatt == 0)
            {
                file << line.toStdString() << endl;
                num = 0;
                line = "";
            }
        }
    }
    file.close();
    cout << "La red se guardo con éxito" << endl;
}

///
/// \brief Función encargada del entrenamiento de la red neuronal para obtener la estructura de la red ARTMAP difusa.
/// \param pathFile     String que representa la ruta del archivo que contiene los patrones de entrenamiento que se usaran.
/// \param saveNet      Booleano que indica si la red neuronal se debe almacenar en un archivo .txt para cargarlo posteriormente.
///
void SystemController::trainNetwork(QString pathFile, bool saveNet)
{
    cout << "Entrenando red neuronal" << endl;

    vector<vector<float> > patterns;
    vector<int> labels;

    ManagerPatterns::getPatternsFromFile(pathFile.toStdString(), patterns, labels);
    int numCls = labels.at(labels.size()-1) + 1;

    cout << ">>> NroTrainPatterns: " << patterns.size() << " - NroCls: " << (numCls) << endl;
    core.trainWithPatterns(patterns, labels, numCls);

    if(saveNet)
    {
        saveInfoNetwork();
    }
}

///
/// \brief Función encargada de cargar la red neuronal ARTMAP difusa a partir de un archivo TXT.
/// \param path     String que indica la ruta y el archivo desde donde se cargará la red neuronal.
///
void SystemController::loadNetworkFrom(QString path)
{
    ifstream file(path.toStdString().c_str());
    string line;
    QString linePat;
    QStringList values;
    vector<vector<float> > infoNet(1);

    int currPos = 0;
    int currLine = 0;

    if( file.is_open() )
    {
        while( getline(file, line) )
        {
            currLine++;
            linePat = QString::fromStdString(line);

            if(currLine < 3)
            {
                infoNet[0].push_back(linePat.toFloat());
                continue;
            }

            if( currLine % 3 == 0)  // Clase del Hiperbox
            {
                infoNet.push_back(vector<float>(1, linePat.toFloat()));
            }
            else
            {
                values = linePat.split(",");
                infoNet.push_back(vector<float>(values.size()-1));
                currPos = infoNet.size() - 1;
                for(int i=0; i<(values.size()-1); i++)
                {
                    infoNet[currPos][i] = values.at(i).toFloat();
                }
            }
        }
        core.loadNetworkConfig(infoNet);
        cout << "Red neuronal cargada" << endl;
    }
    else
        cout << "No se encontro la red neuronal" << endl;
}

///
/// \brief Función encargada de la clasificación a partir de una imagen.
/// \param img  Representa la imagen que se analizará para clasificar.
/// \return     String que representa el nombre del tipo de defecto al que pertenece la imagen.
///
QString SystemController::classifyImage(QImage img)
{
    int cls = core.classifyImage(convertToImage(img));
    return getLabelClass(cls);
}

///
/// \brief Función encargada de la clasificación a partir de la ruta de una imagen.
/// \param path     String que representa el path de la imagen que se desea clasificar.
/// \return         String que representa el nombre del tipo de defecto al que pertenece la imagen.
///
QString SystemController::classifyImage(QString path)
{
    int cls = core.classifyImage(path.toStdString().c_str());
    return getLabelClass(cls);
}

///
/// \brief Función encargada de calcular el patrón característico de una imagen.
/// \param path     String que representa el path de la imagen de la cual se desea calcular el patrón.
/// \return         Vector que contiene los descriptores obtenidos a partir de la imagen.
///
vector<float> SystemController::calculatePattern(QString path)
{
    core.calculatePattern(path.toStdString().c_str());
    return core.getFeatures();
}

///
/// \brief Función encargada de calcular el patrón característico de una imagen.
/// \param img      Imagen de la cual se desea calcular el patrón.
/// \return         Vector que contiene los descriptores obtenidos a partir de la imagen.
///
vector<float> SystemController::calculatePattern(QImage img)
{
    core.calculatePattern(convertToImage(img));
    return core.getFeatures();
}

///
/// \brief Función getter del atributo core.
/// \return     Retorna el core de la clase.
///
SystemCore SystemController::getCore() const
{
    return core;
}

///
/// \brief Función encargada de retornar la imagen de entrada del sistema.
/// \return     Imagen de entrada del sistema.
///
QPixmap SystemController::getImageOriginal()
{
    Image temp = core.getImage(SystemCore::IMAGE_ORIGINAL);
    return QPixmap::fromImage(temp.convertToQimage());
}

///
/// \brief Función encargada de retornar la imagen binaria obtenida después de la segmentación.
/// \return     Imagen binaria del defecto.
///
QPixmap SystemController::getImageBinary()
{
    Image temp = core.getImage(SystemCore::IMAGE_BINARY);
    return QPixmap::fromImage(temp.convertToQimage());
}

///
/// \brief Función encargada de enviar los descriptores y sus respectivos nombres en modo string.
/// \return     String que contiene los nombres y los respectivos valores de cada uno de los descriptores del defecto.
///
QString SystemController::getTextResults()
{
    vector<float> values = core.getFeatures();
    vector<string> names = core.getNamesFeatures();

    int totalFeatures = values.size();
    QString text = "";

    for(int i=0; i < totalFeatures; i++)
    {
        text += QString::fromStdString(names[i]) + ": <b><i>" + QString::number(values[i]) + "</i></b><br></br>";
    }

    return "<p>" + text + "</p>";
}

