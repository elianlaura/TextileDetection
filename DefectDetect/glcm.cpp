/**
 * \file glcm.cpp
 * \brief implementacion del cuerpo de la clase GLCM
 * \author Darwin Saire Pilco
 * \date 05/2014
*/

#include "glcm.h"

GLCM::GLCM()
{
}

int GLCM::compareD( double x, double y ) { return ( x <= y + EPS ) ? ( x + EPS < y ) ? -1 : 0 : 1; }
//double ***Descriptors, **GLCM, **PreSum;

bool GLCM::PreprocesingImageFunction( Mat &img_rgb, Mat &img_gray2, int &bloq, int &scale,
    double*** &Descriptors, double** &GLCM ){

    if( bloq == 0 || bloq == 1 || scale == 0 ) throw 1;
    if( img_rgb.empty() || img_gray2.empty() ) throw 2;
    if( bloq == '\0' || scale == '\0' ) throw 3;

    int h = img_rgb.size().height, w = img_rgb.size().width;
    int interval_h = h / bloq, interval_w = w / bloq;

    Descriptors = new double** [ interval_h ];
    for( int i = 0; i < interval_h; ++i ){
        Descriptors[ i ] = new double* [ interval_w ];
        for( int j = 0; j < interval_w; ++j ) Descriptors[ i ][ j ] = new double[ 9 ];
    }
    if( !img_rgb.data ){ printf( "No puedo abrir o no encontro el archivo\n" ); return false;}

    //int h = img_rgb.size().height, w = img_rgb.size().width;
    //int interval_h = h / bloq, interval_w = w / bloq;
    resize( img_rgb, img_rgb, Size2i( interval_w * bloq + 1, interval_h * bloq + 1 ) );
    resize( img_gray2, img_gray2, Size2i( interval_w * bloq + 1, interval_h * bloq + 1 ) );
    GLCM = new double* [ scale ];
    for( int i = 0; i < scale; ++i ) GLCM[ i ] = new double[ scale ];
    if( Descriptors == 0 || GLCM == 0 ) throw 1;
    return true;
}

void GLCM::ProcesoGLCM( int &scale, Mat &img_gray, double*** &Descriptors, double** &GLCM,
                  int h_ini, int w_ini, int h_fin, int w_fin, int id_h, int id_w ){

    if( scale == 0 ) throw 1;
    if( img_gray.empty() ) throw 2;
    if( scale == '\0' || h_fin == '\0' || w_fin == '\0' ) throw 3;
    if( Descriptors == 0 || GLCM == 0 ) throw 1;

    int scaler = 256 / scale;
    for( int i = 0; i < scale; ++i ) for( int j = 0; j < scale; ++j ) GLCM[ i ][ j ] = 0;
    int pixel_r, pixel_l;
    unsigned char *data = ( unsigned char* )img_gray.data;
    double Sum = 0;
    for( int i = h_ini; i < h_fin; ++i ){
        for( int j = w_ini; j < w_fin - 1; ++j ){
            pixel_l = (int)data[ img_gray.step * i + j ];
            pixel_r = (int)data[ img_gray.step * i + j + 1 ];
            GLCM[ pixel_l / scaler ][ pixel_r / scaler ]++;
            Sum++;
        }
    }

   double P, contrast = 0, homogeneity = 0, entropy = 0, energy = 0,
           mean_i = 0, mean_j = 0, variance_i = 0, variance_j = 0, correlation = 0;
    for( int i = 0; i < scale; ++i ){
        for( int j = 0; j < scale; ++j ){
            P = GLCM[ i ][ j ] / Sum;
            contrast += ( P * ( i - j ) * ( i - j ) );
            homogeneity += ( P / ( 1 + ( i - j ) * ( i - j ) ) );
            energy += P * P;
            if( P > 0.0 ) entropy += ( P * ( -1 * log( P ) ) );
            mean_i += ( i * P );
            mean_j += ( j * P );
        }
    }

    for( int i = 0; i < scale; ++i ){
        for( int j = 0; j < scale; ++j ){
            P = GLCM[ i ][ j ] / Sum;
            variance_i += ( P * ( i - mean_i ) * ( i - mean_i ) );
            variance_j += ( P * ( j - mean_j ) * ( j - mean_j ) );
        }
    }

    for( int i = 0; i < scale; ++i ){
        for( int j = 0; j < scale; ++j ){
            P = GLCM[ i ][ j ] / Sum;
            double v = variance_i * variance_j;
            if( compareD( v, 0.0 ) == 0 ){
                v = 1.0;
            }
            correlation += ( P * ( i - mean_i ) * ( j - mean_j ) / ( v ) );
        }
    }

    Descriptors[ id_h ][ id_w ][ 4 ] = entropy;
    Descriptors[ id_h ][ id_w ][ 5 ] = contrast;
    Descriptors[ id_h ][ id_w ][ 6 ] = homogeneity;
    Descriptors[ id_h ][ id_w ][ 7 ] = correlation;
    Descriptors[ id_h ][ id_w ][ 8 ] = energy;

}

void GLCM::SplitBlock( double *** &Descriptors, double** &GLCM, int &scale, int &bloq, Mat &img_rgb, Mat &img_gray ){
    if( Descriptors == 0 || GLCM == 0 ) throw 1;
    if( scale == 0 || bloq == 0 ) throw 1;
    if( img_rgb.empty() || img_gray.empty() ) throw 2;

    int img_h = img_rgb.size().height, img_w = img_rgb.size().width;
    int interval_h = img_h / bloq, interval_w = img_w / bloq;
    for( int i = 0, range_h = 0; i < interval_h; ++i, range_h += bloq ){
        for( int j = 0, range_w = 0; j < interval_w; ++j, range_w += bloq ){
            Descriptors[ i ][ j ][ 0 ] = range_h;
            Descriptors[ i ][ j ][ 1 ] = range_w;
            Descriptors[ i ][ j ][ 2 ] = range_h + bloq;
            Descriptors[ i ][ j ][ 3 ] = range_w + bloq;
            ProcesoGLCM( scale, img_gray, Descriptors, GLCM,
                         range_h, range_w, range_h + bloq, range_w + bloq, i, j );
        }
    }
}

bool GLCM::IndentifyDefect( Mat &img_rgb, Mat &img_entropy, Mat &img_contrast, Mat &img_homogeneity,
                      Mat &img_correlation, Mat& img_energy, Mat& UnionImage, double ***Descriptors, int bloq ){

    bool detect_defect = false;
    if( img_rgb.empty() ) throw 2;
    if( Descriptors == 0 || bloq == 0 ) throw 1;

    int num_desc = 5;
    double max_desc[ num_desc ], min_desc[ num_desc ], mean_desc[ num_desc ], standar_desc[ num_desc ],
           bound_desc[ num_desc ][ 2 ]; int count_desc[ num_desc ][ 2 ]; // 0 min 1 max
    Mat image[ num_desc ];

    for( int i = 0; i < num_desc; ++i ){ max_desc[ i ] = -INF; min_desc[ i ] = INF; count_desc[ i ][ 0 ] = 0; count_desc[ i ][ 1 ] = 0;
                                  mean_desc[ i ] = 0; bound_desc[ i ][ 0 ] = 0; bound_desc[ i ][ 1 ] = 0; standar_desc[ i ] = 0; }

    int img_h = img_rgb.size().height, img_w = img_rgb.size().width;
    int interval_h = img_h / bloq, interval_w = img_w / bloq;

    for( int i = 0; i < interval_h; ++i )
        for( int j = 0; j < interval_w; ++j )
            for( int k = 0; k < num_desc; ++k ){
                if( compareD( Descriptors[ i ][ j ][ k + 4 ], max_desc[ k ] ) == 1 )
                    max_desc[ k ] = Descriptors[ i ][ j ][ k + 4 ];
                if( compareD( Descriptors[ i ][ j ][ k + 4 ], min_desc[ k ] ) == -1 )
                    min_desc[ k ] = Descriptors[ i ][ j ][ k + 4 ];
                mean_desc[ k ] += Descriptors[ i ][ j ][ k + 4 ];
            }

    /*for( int i = 0; i < interval_h; ++i )
        for( int j = 0; j < interval_w; ++j )
            for( int k = 0; k < num_desc; ++k ){
                if( compareD( min_desc[ k ], Descriptors[ i ][ j ][ k + 4 ] ) == 0 ) count_desc[ k ][ 0 ]++;
                if( compareD( max_desc[ k ], Descriptors[ i ][ j ][ k + 4 ] ) == 0 ) count_desc[ k ][ 1 ]++;
            }*/

    for( int i = 0; i < num_desc; ++i )
        mean_desc[ i ] /= ( interval_h * interval_w );

    for( int i = 0; i < interval_h; ++i ){
        for( int j = 0; j < interval_w; ++j ){
            for( int k = 0; k < num_desc; ++k ){
                standar_desc[ k ] += ( ( Descriptors[ i ][ j ][ k + 4 ] - mean_desc[ k ] )
                        * ( Descriptors[ i ][ j ][ k + 4 ] - mean_desc[ k ] ) );
            }
        }
    }

    for( int i = 0; i < num_desc; ++i ){
        standar_desc[ i ] /= ( interval_h * interval_w );
        standar_desc[ i ] = sqrt( standar_desc[ i ] );
    }

    // intervalo de confianza
    double value_crit = 0;
    for( int i = 0; i < num_desc; ++i ){
        if( compareD( standar_desc[ i ], 0.0 ) != 0 ){
            mean_desc[ i ] /= ( bloq * bloq );
            value_crit = 3.5 * standar_desc[ i ];
            //printf("%.5f\n", value_crit );
            /*cout << "mean: " << i << " " << mean_desc[ i ] << endl;
            cout << "standar: " << i << " " << standar_desc[ i ] << endl;
            cout << "value_crit: " << i << " " << value_crit << endl;*/
            bound_desc[ i ][ 0 ] = mean_desc[ i ] - value_crit;
            bound_desc[ i ][ 1 ] = mean_desc[ i ] + value_crit;
        }
    }


    // intervalo por rango
    /*double range = 0;
    for( int i = 2; i <= 2; ++i ){
        //if( compareD( standar_desc[ i ], 0.0 ) == 0 ){
            range = ( max_desc[ i ] - min_desc[ i ] ) / 2;// ( count_desc[ i ][ 0 ] + count_desc[ i ][ 1 ] ) ;
            bound_desc[ i ][ 0 ] = mean_desc[ i ] - range;
            bound_desc[ i ][ 1 ] = mean_desc[ i ] + range;
        //}
    }*/

    for( int i = 0; i < num_desc; ++i ) img_rgb.copyTo( image[ i ] );
    Scalar color = Scalar(255,0,0);
    Scalar color1 = Scalar(0,255,0);
    Scalar color2 = Scalar(0,0,255);

    Mat UnionImg;
    img_rgb.copyTo( UnionImg );



    bool b_ent, b_cont, b_hom, b_ene;
    vector< pair< int, int > > points;
    bool **block_defect;
    int counter = 0;
    block_defect = new bool*[ interval_h ];
    for( int i = 0; i < interval_h; ++i ){
        block_defect[ i ] = new bool[ interval_w ];
    }

    for( int i = 0; i < interval_h; ++i ){
       for( int j = 0; j < interval_w; ++j )
            block_defect[ i ][ j ] = false;
    }

    for( int i = 0; i < interval_h; ++i )
        for( int j = 0; j < interval_w; ++j ){
            b_ent = b_cont = b_hom = b_ene = false;
            for( int k = 0; k < num_desc; ++k ){                
                if( compareD( Descriptors[ i ][ j ][ k + 4 ], bound_desc[ k ][ 0 ] ) == -1 ||
                        compareD( Descriptors[ i ][ j ][ k + 4 ], bound_desc[ k ][ 1 ] ) == 1 ){
                    //cout<<"bbbbbbbbbbb"<<endl;
                    if( k == 0 ) b_ent = true; if( k == 1 ) b_cont = true;
                    if( k == 2 ) b_hom = true; if( k == 4 ) b_ene = true;
                    rectangle( image[ k ], cvPoint( (int)Descriptors[ i ][ j ][ 1 ] , (int)Descriptors[ i ][ j ][ 0 ] ),
                            cvPoint( (int)Descriptors[ i ][ j ][ 3 ],(int)Descriptors[ i ][ j ][ 2 ]), color, 2, 8, 0 );//-1, 8 );
                }
            }
            if( ( b_cont == true && b_hom == true ) ){

                block_defect[ i ][ j ] = true;
                counter++;
                points.push_back( ii( (int)Descriptors[ i ][ j ][ 0 ], (int)(int)Descriptors[ i ][ j ][ 1 ] ) );
                rectangle( UnionImg, cvPoint( (int)Descriptors[ i ][ j ][ 1 ] , (int)Descriptors[ i ][ j ][ 0 ] ),
                            cvPoint( (int)Descriptors[ i ][ j ][ 3 ],(int)Descriptors[ i ][ j ][ 2 ]), color1, 2, 8, 0 );
            }
        }


    //set<Point> sp;
    CFoodFill obj( interval_h, interval_w );
    obj.setMatrix( block_defect );
    vector<Point> vp;
    //set<Point>::iterator iter_sp;
    //Grafo grafo( points );
    //cout << "point_size: " << points.size() << endl;
    //if( grafo.SlimDow() && points.size() > 4 && points.size() < 9){
    if( obj.func_FoodFill( points.size() ) &&  points.size() > 1 && points.size() < 9){
        //cout << "Size_img: " << interval_h << " " << interval_w << endl;
        //cout << "point_size: " << points.size() << endl;
        //DFS para reemplazar la busqueda de O(n^2), cuando haya tiempo
        detect_defect = true;
        for( int i = 0; i < interval_h; ++i )
            for( int j = 0; j < interval_w; ++j ){
                b_ent = b_cont = b_hom = b_ene = false;
                for( int k = 0; k < num_desc; ++k ){
                    if( compareD( Descriptors[ i ][ j ][ k + 4 ], bound_desc[ k ][ 0 ] ) == -1 ||
                            compareD( Descriptors[ i ][ j ][ k + 4 ], bound_desc[ k ][ 1 ] ) == 1 ){
                        //cout<<"aaaaaaaa"<<endl;
                        if( k == 0 ) b_ent = true; if( k == 1 ) b_cont = true;
                        if( k == 2 ) b_hom = true; if( k == 4 ) b_ene = true;
                        rectangle( image[ k ], cvPoint( (int)Descriptors[ i ][ j ][ 1 ] , (int)Descriptors[ i ][ j ][ 0 ] ),
                                cvPoint( (int)Descriptors[ i ][ j ][ 3 ],(int)Descriptors[ i ][ j ][ 2 ]), color2, 2, 8, 0 );//-1, 8 );
                    }
                }
                if( ( b_cont == true && b_hom == true ) ){
                    cout << "------------------------------\n";
                    int y = (int)Descriptors[ i ][ j ][ 0 ];
                    int x  = (int)Descriptors[ i ][ j ][ 1 ];
                    Point p_[ 4 ];
                    p_[ 0 ].x = x; p_[ 0 ].y = y;
                    p_[ 1 ].x = x + bloq; p_[ 1 ].y = y;
                    p_[ 2 ].x = x; p_[ 2 ].y = y +  bloq;
                    p_[ 3 ].x = x + bloq; p_[ 3 ].y = y + bloq;
                    for( int m = 0; m < 4; ++m ) vp.push_back( p_[ m ] );
                    rectangle( UnionImg, cvPoint( (int)Descriptors[ i ][ j ][ 1 ] , (int)Descriptors[ i ][ j ][ 0 ] ),
                                cvPoint( (int)Descriptors[ i ][ j ][ 3 ],(int)Descriptors[ i ][ j ][ 2 ]), color2, 2, 8, 0 );
                }
            }

        vector<Point> hull( vp.size() );
        convexHull( vp, hull, CV_CLOCKWISE, 0 );

        Point pt0 = hull[ hull.size() - 1 ];

        for( int i = 0; i < (int)hull.size(); i++ ) {
          Point pt = hull[ i ];
          line( UnionImg, pt0, pt, CV_RGB( 255, 0, 0 ), 2, CV_AA, 0 );
          pt0 = pt;
        }
    }


    img_entropy = image[ 0 ];
    img_contrast = image[ 1 ];
    img_homogeneity = image[ 2 ];
    img_correlation = image[ 3 ];
    img_energy = image[ 4 ];
    UnionImage = UnionImg;

    if( img_entropy.empty() || img_contrast.empty() || img_homogeneity.empty() || img_correlation.empty()
        || img_energy.empty() || UnionImage.empty() ) throw 2;

    return detect_defect ;
}


void GLCM::PrintfDescriptors( double*** &Descriptors, int bloq ){
    if( bloq == 0 || Descriptors == 0 ) throw 1;
    if( bloq == '\0' ) throw 3;
    for( int i = 0; i < bloq; ++i ){
        for( int j = 0; j < bloq; ++j ) {
            printf( "bloque: %d %d %.4f %.4f %.4f %.4f\n", i, j, Descriptors[ i ][ j ][ 4 ],
                    Descriptors[ i ][ j ][ 5 ], Descriptors[ i ][ j ][ 6 ], Descriptors[ i ][ j ][ 7 ] );
        }
        cout << endl;
    }
}

