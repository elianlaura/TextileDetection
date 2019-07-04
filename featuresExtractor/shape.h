#ifndef SHAPE_H
#define SHAPE_H

#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

///
/// \brief La clase Shape representa la forma (defecto) en una imagen para obtener posteriormente sus características.
///
class Shape
{
private:
    vector<Point> contour;
    vector<vector<Point> > regions;
    int numRegions;
    float area;
    float areaRegions;
    float perimeter;
    float maxDiameter;
    Rect box;
    Point centroid;
    Point2f geometricCenter;

    Point calculateCentroid();
    float calculateAreaRegions();

public:
    Shape() {}
    void setContour(vector<Point> contour, vector<vector<Point> > regions);
    float calculateCircularity();
    float calculateRoundness();
    float calculateAspectRatio();
    float calculateExtent();
    float calculateSolidity();
    float calculateCompactness();
    float calculateSymmetry();
    float calculateElongation(int numErosions);
    float calculateRatioWidth(int widthImage);
    float calculateRatioHeight(int heightImage);
    float calculateRatioImage(int widthImage, int heightImage);

    float getNumRegions();
    Rect getBox();
    Point getCentroid();
    Point2f getGeometricCenter();
    vector<Point> getContour();
    vector<vector<Point> > getRegions();
};

#endif // SHAPE_H


