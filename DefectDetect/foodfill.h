#ifndef FOODFILL_H
#define FOODFILL_H
#include <utility>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class CFoodFill
{
    public:
        CFoodFill( int, int );
        int FoodFill( int, int, bool );
        bool func_FoodFill( int );
        pair< int, int > find_block_defect( void );
        void setMatrix( bool **mtr );

    private:
        int row;
        int col;
        bool **matrix;
        bool **visit;
        int cx[ 4 ];// = {1,-1, 0, 0};
        int cy[ 4 ];// = {0, 0, 1,-1};
};
#endif // FOODFILL_H
