/**
 * \file grafo.cpp
 * \brief implementacion del cuerpo de la clase Grafo
 * \author Darwin Saire Pilco
 * \date 05/2014
*/

#include "grafo.h"

Grafo::Grafo( const vector< pair< int, int > >& points )
{
     diameter = -1000000000;
     vector< pair< int, id > > EdgeList;
     V = points.size();
     E = 0;
     //E = ( V - 1 ) * ( V - 2 ) / 2;
     AdjList.assign( V, vid() );
     AdjListMST.assign( V, vid() );
     for( int i = 0; i < V - 1; ++i )
         for( int j = i + 1; j < V; j++ ){
             E++;
             w = distance( points[ i ], points[ j ] );
             diameter = fmax( diameter, w );
             u = i; v = j;
             EdgeList.push_back(make_pair( w, id( u, v ) ) );
             AdjList[ u ].push_back( id( v, w ) );
             AdjList[ v ].push_back( id( u, w ) );
         }
     pset.resize( E );
     setSize.reserve( E );
     sort( EdgeList.begin(), EdgeList.end() );
     int mst_cost = 0; initSet( V );
     for ( int i = 0; i < E; i++ ){
         pair< int, id > front = EdgeList[ i ];
         if( !isSameSet( front.second.first, front.second.second ) ) {
             AdjListMST[ front.second.first ].push_back( id( front.second.second, front.first ) );
             mst_cost += front.first;
             unionSet(front.second.first, front.second.second);
         }
     }
     radio = (mst_cost)*0.55;
     //printf( "Diamater: %f  MST: %d  Radio: %f\n", diameter , mst_cost, radio );
}

double Grafo::distance( id p1, id p2 )
{
    double dist = 0;
    dist += ( p1.first - p2.first ) * ( p1.first - p2.first );
    dist += ( p1.second - p2.second ) * ( p1.second - p2.second );
    return sqrtf( dist );
}

void Grafo::initSet( int N )
{
   setSize.assign( N, 1 ); _numDisjointSets = N;
   pset.assign( N, 0 );
   for( int i = 0; i < N; i++ ) pset[ i ] = i;
}

int Grafo::findSet( int i )
{
    return ( pset[ i ] == i ) ? i : ( pset[ i ] = findSet( pset[ i ] ) );
}

bool Grafo::isSameSet( int i, int j )
{
    return findSet( i ) == findSet( j );
}

void Grafo::unionSet( int i, int j )
{
   if( !isSameSet( i, j ) ){
      _numDisjointSets--;
      setSize[ findSet( j ) ] += setSize[ findSet( i ) ];
      pset[ findSet( i ) ] = findSet( j );
   }
}

int Grafo::DFS( int u )
{
    visit[ u ] = 1;
    int ans = 0;
    for( int i = 0; i < (int)AdjListMST[ u ].size(); ++i ){
        id point = AdjListMST[ u ][ i ];
        if( point.second >= radio ) { ans++; printf( "%f %f\n", point.second, radio ); }
        if( visit[ point.first ] == 0 ) ans += DFS( point.first );
    }
    return ans;
}

bool Grafo::SlimDow( void )
{
    if( V == 0 || E == 0 ) return true;
    visit.assign( E, 0 );
    int sum = ( 0 );
    //printf( "DFS: %d\n", sum );
    if( sum > 0 ) return false;
    else return true;
}
