#include "foodfill.h"

CFoodFill::CFoodFill( int x, int y )
{
    int cx_t[] = {1,-1, 0, 0};
    int cy_t[] = {0, 0, 1,-1};
    for( int k = 0; k < 4; ++k ){
        cx[ k ] = cx_t[ k ];
        cy[ k ] = cy_t[ k ];
    }

    this->row = x;
    this->col = y;
    matrix = new bool*[ row ];
    visit = new bool*[ row ];
    for( int i = 0; i < row; i++ ){
        visit[ i ] = new bool[ col ];
        matrix[ i ] = new bool[ col ];
        for( int j = 0; j < col; ++j ) {
            visit[ i ][ j ] = false;
            matrix[ i ][ j ]= false;
        }
    }
}

void CFoodFill::setMatrix( bool **mtr ){
    this->matrix = mtr;
}

pair< int, int > CFoodFill::find_block_defect( void ){
    for( int i = 0; i < row; ++i )
        for( int j = 0; j < col; ++j )
            if( matrix[ i ][ j ] ) return make_pair( i, j );
    return make_pair( -1, -1 );
}

int CFoodFill::FoodFill( int x, int y, bool have_defect ){
    visit[ x ][ y ] = true;
    int ans = 0;
    for( int k = 0; k < 4; ++k ){
        int xi = x + cx[ k ];
        int yi = y + cy[ k ];
        if( xi >= 0 && xi < row && yi >= 0 && yi < col && !visit[ xi ][ yi ] ){
            if( matrix[ xi ][ yi ] ) ans += 1 + FoodFill( xi, yi, true );
            else {
                if( have_defect ) ans += FoodFill( xi, yi, false );
            }
        }
    }
    return ans;
}

bool CFoodFill::func_FoodFill( int count_defect ){
    int counter_defect = 0;
    pair< int, int > pi = find_block_defect();
    if( pi.first == -1 || pi.second == -1 ) { return true; }
    else{
                //cout << "init: " << pi.first << " " << pi.second << endl;
        counter_defect += 1 + FoodFill( pi.first, pi.second, true );
    }
    //cout << "counter_defect: " << counter_defect << endl;
    //cout << "count_defect: " << count_defect << endl;
    return counter_defect == count_defect ? true : false;
}
