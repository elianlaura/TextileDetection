/**
 * \file glcm.h
 * \brief implementacion de Gray-Level Co-occurrence Matrices
 * \author Darwin Saire Pilco
 * \date modificado 05/2014
*/


#ifndef GLCM_H
#define GLCM_H

#include <cv.h>
#include <highgui.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <set>
#include <cstdio>
#include <vector>
#include <queue>

#include "grafo.h"
#include "foodfill.h"

using namespace cv;
using namespace std;

#define EPS 1e-3
#define INF 1000000000

/**
 * \class GLCM
 * \brief clase de GLCM
*/

class GLCM
{
    public:

        /**
         * \fn GLCM
         * \brief constructor de GLCM
        */
        GLCM();

        /**
         * \fn compareD
         * \brief compara dos double
         * \param x numero double
         * \param y numero double
         * \return -1 x < y ;
         *          0 x == y ;
         *          1 x > y ;
         */
        int compareD( double x, double y );

        /**
         * \fn PreprocesingImageFunction
         * \brief iniciar las variables y convertir a gris la image
         * \param img_rgb imagen en rgb
         * \param img_gray2 imagen en griss
         * \param bloq tamanio de bloque
         * \param scale tamanio de matrix glcm scale * scale
         * \param Descriptors matrix que guarda los descriptores
         * \param GLCM matrix GLCM
         * \return true si se realizo correctamente la inicializacion
         */
        bool PreprocesingImageFunction( Mat &img_rgb, Mat &img_gray2, int &bloq, int &scale,
            double*** &Descriptors, double** &GLCM );

        /**
         * \fn ProcesoGLCM
         * \brief obtener los decriptores del glcm y guardarlos en la matrix glcm
         * \param scale tamanio de matrix glcm scale * scale
         * \param img_gray imagen gris
         * \param Descriptors matrix que guarda los descriptores
         * \param GLCM matrix GLCM
         * \param h_ini altura inicial del bloque
         * \param w_ini ancho inicial del bloque
         * \param h_fin altura final del bloque
         * \param w_fin ancho final del bloque
         * \param id_h indice de altura del bloque
         * \param id_w indice de ancho del bloque
        */
        void ProcesoGLCM( int &scale, Mat &img_gray, double*** &Descriptors, double** &GLCM,
                          int h_ini, int w_ini, int h_fin, int w_fin, int id_h, int id_w );

        /**
         * \fn SplitBlock
         * \brief dividir la imagen en cantidad de bloques
         * \param Descriptors matrix que guarda los descriptores
         * \param GLCM matrix GLCM
         * \param scale tamanio de matrix glcm scale * scale
         * \param bloq tamanio del bloque
         * \param img_rgb imagen en rgb
         * \param img_gray imagen en gris
         */
        void SplitBlock( double *** &Descriptors, double** &GLCM, int &scale, int &bloq, Mat &img_rgb, Mat &img_gray );

        /**
         * \fn IndentifyDefect
         * \brief deteccion de defectos con MST y convexhull
         * \param img_rgb imagen en rgb
         * \param img_entropy imagen detectado con descriptor entropia
         * \param img_contrast imagen detectado con descriptor contraste
         * \param img_homogeneity imagen detectado con descriptor homogeneidad
         * \param img_correlation imagen detectado con descriptor correlacion
         * \param img_energy imagen detectado con descriptor energia
         * \param UnionImage imagen detectado con descriptor union
         * \param Descriptors matrix que guarda los descriptores
         * \param bloq tamanio del bloque
         */
        bool IndentifyDefect( Mat &img_rgb, Mat &img_entropy, Mat &img_contrast, Mat &img_homogeneity,
                              Mat &img_correlation, Mat& img_energy, Mat& UnionImage, double ***Descriptors, int bloq );

        /**
         * \fn PrintfDescriptors
         * \brief imprimir la matrix GLCM
         * \param Descriptors matrix que guarda los descriptores
         * \param bloq tamanio del bloque
         */
        void PrintfDescriptors( double*** &Descriptors, int bloq );
};

#endif // GLCM_H
