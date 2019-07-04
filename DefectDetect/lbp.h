/**
 * \file lbp.h
 * \brief implementacion de local binary pattern
 * \author internet
 * \date modificado 05/2014
*/

#ifndef LBP_H
#define LBP_H

#include <cv.h>
#include <limits>

using namespace cv;
using namespace std;

/**
 * \class LBP
 * \brief clase que implementa LBP
*/
class LBP
{
public:
        /**
         * \fn LBP
         * \brief constructor del LBP
        */
        LBP();

        /**
         * \fn OLBP_
         * \brief ejecuta el overlapped local binary pattern
         * \details aplica el filtro OLBP a dst
         * \param src imagen fuente
         * \param dst imagen destino
        */
        void OLBP_(const cv::Mat& src, cv::Mat& dst);

         /**
         * \fn ELBP_
         * \brief ejecuta el elongated local binary pattern
         * \details aplica el filtro ELBP a dst
         * \param src imagen fuente
         * \param dst imagen destino
         * \param radius tamanio del radio
         * \param neighbors cantidad de vecinos
        */
        void ELBP_(const cv::Mat& src, cv::Mat& dst, int radius = 1, int neighbors = 8);

        /**
         * \fn VARLBP_
         * \brief ejecuta el variance local binary pattern
         * \details aplica el filtro VARLBP a dst
         * \param src imagen fuente
         * \param dst imagen destino
         * \param radius tamanio del radio
         * \param neighbors cantidad de vecinos
        */
        void VARLBP_(const cv::Mat& src, cv::Mat& dst, int radius = 1, int neighbors = 8);
        
        /**
         * \fn OLBP
         * \brief llama a la funcion OLBP_
         * \param src imagen a filtrar
         * \return imagen filtrada con OLBP
        */
        Mat OLBP(const Mat& src);

        /**
         * \fn ELBP
         * \brief llama a la funcion ELBP_
         * \param src imagen fuente
         * \param radius tamanio del radio
         * \param neighbors cantidad de vecinos
         * \return imagen filtrada con ELBP 
        */
        Mat ELBP(const Mat& src, int radius = 1, int neighbors = 8);

        /**
         * \fn VARLBP
         * \brief llama a la funcion VARLBP_
         * \param src imagen fuente
         * \param radius tamanio del radio
         * \param neighbors cantidad de vecinos
         * \return imagen filtrada con VARLBP 
        */
        Mat VARLBP(const Mat& src, int radius = 1, int neighbors = 8);

};

#endif // LBP_H
