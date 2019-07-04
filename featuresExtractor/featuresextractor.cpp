#include "featuresextractor.h"
#include "../DefectClassifier/imagehelper.h"

bool comparePoints(Point point1, Point point2)
{
    double i = point1.x;
    double j = point2.x;

    if(i == j) {
        i = point1.y;
        j = point2.y;
    }
    return ( i < j );
}

int cross(Point O, Point A, Point B)
{
    if((A.x - O.x) == 0)
        if(A.y < B.y)
            return 0;
        else return -1;

    float pendiente1 = (A.y - O.y) / (A.x - O.x);
    float pendiente2 = (B.y - O.y) / (B.x - O.x);

    if(pendiente1 > pendiente2)
        return 0;
    else
        return 1;
}

///
/// \brief Esta función retorna los nombres de los descriptores calculados.
/// \return Vector que contiene los nombres de los descriptores.
///
vector<string> FeaturesExtractor::getNames() const
{
    return names;
}

///
/// \brief Esta función retorna los valores de los descriptores de forma calculados.
/// \return Vector que contiene los valores (float) de los descriptores.
///
vector<float> FeaturesExtractor::getValues() const
{
    return values;
}

///
/// \brief Esta función se encarga de calcular los descriptores de forma a partir de una imagen binaria.
/// \param image        Imagen binaria del defecto.
/// \param imgWidth     Entero que representa el ancho de la imagen de entrada (imagen a colores).
/// \param imgHeight    Entero que representa el largo de la imagen de entrada.
///
void FeaturesExtractor::getFeaturesShape(Image image, int imgWidth, int imgHeight)
{
    cout << ">>> Obteniendo descriptores ..." << endl;
    values.clear();
    names.clear();

    Shape shape;
    bool existsShape = getShape(image, shape);

    if(existsShape)
    {
        names.push_back("Nro Regiones");
        values.push_back(shape.getNumRegions());
        names.push_back("Excentricidad");
        values.push_back(shape.calculateAspectRatio());
        names.push_back("Circularidad");
        values.push_back(shape.calculateCircularity());
        //names.push_back("Roundness");
        //values.push_back(shape.calculateRoundness());
        //names.push_back("Extent");
        //values.push_back(shape.calculateExtent());
        names.push_back("Solidez");
        values.push_back(shape.calculateSolidity());
        //names.push_back("Symmetry");
        //values.push_back(shape.calculateSymmetry());
        //names.push_back("Compactness");
        //values.push_back(shape.calculateCompactness());
        //names.push_back("Thinness");
        //values.push_back(shape.calculateThinness());

        /*
        names.push_back("RatioWidth");
        values.push_back(shape.calculateRatioWidth(imgWidh));
        names.push_back("RatioHeight");
        values.push_back(shape.calculateRatioHeight(imgHeight));
        */
        names.push_back("RatioImage");
        values.push_back(shape.calculateRatioImage(imgWidth, imgHeight));

        //Mat finalImage;
        //Image::drawImageContours(this->image, finalImage, shape.getCentroid(), shape.getBox(), shape.getContour(), shape.getRegions());
        //Image::drawImageContours(this->image, finalImage, shape.getCentroid(), shape.getBox(), hull, shape.getRegions());
        //Image::showImageInWindow("Contours", finalImage);
    }
    else
    {
        names.push_back("Nro Regiones");
        values.push_back(0);
        names.push_back("Excentricidad");
        values.push_back(0);
        names.push_back("Circularidad");
        values.push_back(0);
        names.push_back("Solidez");
        values.push_back(0);
        names.push_back("RatioImage");
        values.push_back(1);
    }
}

///
/// \brief Esta función se encarga de calcular el contorno de todo el defecto (similar convex hull).
/// \param points           Contiene los puntos de todos los contornos de las regiones de un defecto.
/// \param contourGeneral   Vector que almacenará los puntos del contorno de todo el defecto.
///
void FeaturesExtractor::getContourGeneral(vector<Point> points, vector<Point> &contourGeneral)
{
    int k=0;
    vector<Point> contour(2 * points.size());
    sort(points.begin(), points.end(), comparePoints);

    // Lower
    for(int i=0; i<points.size(); i++)
    {
        while(k >= 2 && cross(contour[k-2], contour[k-1], points[i]) <= 0)
            k--;
        contour[k++] = points[i];
    }

    // Upper
    for(int i=points.size()-2, t=k+1; i >=0; i--)
    {
        while(k >= t && cross(contour[k-2], contour[k-1], points[i]) <= 0)
            k--;
        contour[k++] = points[i];
    }

    contour.resize(k);
    contourGeneral = contour;
}

///
/// \brief FeaturesExtractor::getShape
/// \param shape        Es el Shape donde se guardara la forma en caso que exista
/// \return             Indica si existe la forma o no
///
bool FeaturesExtractor::getShape(Image image, Shape &shape)
{
    Mat tempImg = image.clone();
    vector<Point> points;
    vector<vector<Point> > contours;
    vector<Point> contourGeneral;

    findContours(tempImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

    for(int i=0; i<contours.size(); i++)
    {
        points.insert(points.end(), contours[i].begin(), contours[i].end());
    }
    getContourGeneral(points, contourGeneral);

    if(contourGeneral.size() == 0)
        return false;

    shape.setContour(contourGeneral, contours);
    return true;
}

