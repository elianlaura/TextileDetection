#include "shape.h"

#define MAX_REG_DEF     1000

///
/// \brief Función que asignará el contorno de todo el defecto y las diferentes regiones que lo conforman.
/// \param contour      Contorno que contiene todo el defecto (similar convex hull).
/// \param regions      Vector que contiene los contornos de las diferentes regiones que conforman el defecto.
///
void Shape::setContour(vector<Point> contour, vector<vector<Point> > regions)
{
    this->contour = contour;
    this->regions = regions;
    this->numRegions = regions.size();
    this->areaRegions = calculateAreaRegions();
    this->area = contourArea(contour);
    this->perimeter = arcLength(contour, true);
    this->box = boundingRect(contour);
    this->centroid = calculateCentroid();

    float radius;
    minEnclosingCircle( (Mat)contour, this->geometricCenter, radius );
    this->maxDiameter = 2 * radius;
}

///
/// \brief Función encarga de calcular el centroide de todo el defecto.
/// \return     Punto donde se encuentra el centroide del defecto.
///
Point Shape::calculateCentroid()
{
    Moments moment = moments(contour, true);
    float cmx = moment.m10/moment.m00;
    float cmy = moment.m01/moment.m00;
    return Point(cmx, cmy);
}

///
/// \brief Función encargada de calcular el área total del defecto, tomando en cuenta sus regiones.
/// \return Valor que indica el área del defecto.
///
float Shape::calculateAreaRegions()
{
    float area = 0;
    for(int i=0; i<numRegions; i++)
    {
        area += contourArea(regions[i]);
    }
    return area;
}

///
/// \brief Función encargada de calcular la relación de circularidad del defecto.
/// \return     Valor de la relación de circularidad del defecto.
///
float Shape::calculateCircularity()
{
    return (4 * M_PI * area) / (perimeter * perimeter);
}

///
/// \brief Función encargada de calcular el factor de redondez del defecto.
/// \return     Valor del factor de redondez.
///
float Shape::calculateRoundness()
{
    return (4 * area) / (M_PI * maxDiameter * maxDiameter);
}

///
/// \brief Función encargada de calcular la relación de aspecto del defecto.
/// \return     Valor de la relación de aspecto.
///
float Shape::calculateAspectRatio()
{
    //return float(box.width) / box.height;
    return std::min(box.width, box.height) * 1.0 / std::max(box.width, box.height);
}

///
/// \brief Función encargada de calcular la extensión del defecto.
/// \return     Valor del factor de extensión.
///
float Shape::calculateExtent()
{
    return areaRegions / (box.height * box.width);
}

///
/// \brief Función encargada de calcular el factor de solidez del defecto.
/// \return     Valor del factor de solidez.
///
float Shape::calculateSolidity()
{
    return areaRegions / area;
}

///
/// \brief Función encargada de calcular el factor de compacidad del defecto.
/// \return     Valor del factor de compacidad.
///
float Shape::calculateCompactness()
{
    return sqrt((4 / M_PI) * area) / maxDiameter;
}

// Si el valor es cercano a 0, tiende a ser simetrica [Russ: Page 600-602]
///
/// \brief Función encargada de calcular el factor de simetría
/// \return Valor del factor de simetría. Si el valor es cercano a 0 indica que tiende a ser simétrico el defecto.
///
float Shape::calculateSymmetry()
{
    float dist = sqrt((centroid.x - geometricCenter.x) * (centroid.x - geometricCenter.x) + (centroid.y - geometricCenter.y) * (centroid.y - geometricCenter.y));
    return dist / (maxDiameter / 2);
}

///
/// \brief Función encargada de calcular la relación entre el ancho del defecto y ancho de la imagen.
/// \param widthImage   Entero que indica el ancho de la imagen.
/// \return             Valor de la relación entre los anchos de la imagen y del defecto.
///
float Shape::calculateRatioWidth(int widthImage)
{
    return box.width * 1.0 / widthImage;
}

///
/// \brief Función encargada de calcular la relación entre el alto del defecto y alto de la imagen.
/// \param heightImage  Entero que indica el alto de la imagen.
/// \return             Valor de la relación entre los altos de la imagen y del defecto.
///
float Shape::calculateRatioHeight(int heightImage)
{
    return box.height * 1.0 / heightImage;
}

///
/// \brief Función encargada de entregar el factor máximo entre la relación de anchos y la relación de altos.
/// \param widthImage       Entero que indica el ancho de la imagen.
/// \param heightImage      Entero que indica el alto de la imagen.
/// \return                 Valor máximo entre los factores mencionados.
///
float Shape::calculateRatioImage(int widthImage, int heightImage)
{
    return std::max(calculateRatioWidth(widthImage), calculateRatioHeight(heightImage));
}

///
/// \brief Esta función devuelve el número de regiones del defecto sobre 1000.
/// \return     Valor de la relación del número de defectos y 1000.
/// Por ejemplo: si el número de regiones es 5, el resultado de esta función sería 0.005
///
float Shape::getNumRegions()
{
    return numRegions * 1.0 / MAX_REG_DEF;
}

///
/// \brief Esta función devuelve el Rect que encierra al defecto.
/// \return     Rect que representa el rectángulo que encierra al defecto.
///
Rect Shape::getBox()
{
    return box;
}

///
/// \brief Esta función devuelve el centroide del defecto.
/// \return     Punto que representa el centroide del defecto.
///
Point Shape::getCentroid()
{
    return centroid;
}

///
/// \brief Esta función devuelve el centro geométrico del defecto.
/// \return     Punto que representa el centro geométrico dentro del defecto.
///
Point2f Shape::getGeometricCenter()
{
    return geometricCenter;
}

///
/// \brief Función que devuelve el contorno de todo el defecto.
/// \return     Vector de puntos que conforman el contorno de todo el defecto.
///
vector<Point> Shape::getContour()
{
    return contour;
}

///
/// \brief Función que devuelve los contornos de las regiones que conforman el defecto.
/// \return     Vector que contiene los contornos de las diferentes regiones que conforman el defecto.
///
vector<vector<Point> > Shape::getRegions()
{
    return regions;
}

///
/// \brief Función que se encarga de calcular la elongación del defecto.
/// \param numErosions  Entero que indica el número de erosiones que se aplica a la imagen para que desaparezca todo el defecto.
/// \return             Valor del factor de elongación.
///
float Shape::calculateElongation(int numErosions)
{
    return areaRegions * 1.0 / (4.0*numErosions*numErosions);
}

