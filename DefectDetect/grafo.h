/**
 * \file grafo.h
 * \brief implementacion simple de grafo con listas enlazadas
 * \author Darwin Saire Pilco
 * \date 05/2014
*/

#ifndef GRAFO_H
#define GRAFO_H

#include <algorithm>
#include <cstdio>
#include <vector>
#include <queue>
#include <math.h>
using namespace std;

typedef pair<int, int> ii;
typedef pair<int, double> id;
typedef vector<int> vi;
typedef vector<id> vid;

/**
 * \class Grafo
 * \brief clase Grafo con listas enlazadas 
*/
class Grafo
{
    public:
        /**
         * \fn Grafo
         * \brief constructor de la clase Grafo
         * \param points vector de pares de enteros que contiene los vertices del grafo
         * \details reservamos memoria para la cantidad de vertices que vamos a manejar y construccion del MST
        */
        Grafo(const vector< pair< int, int > >& points);

        /**
         * \fn initSet
         * \brief inicializamos los padres y componente conexas
         * \param N cantidad de vertices que vamos a manejar
        */
        void initSet( int N );

        /**
         * \fn findSet
         * \brief encontrar la componente conexa a la que pertenece el vertice i
         * \param vertice i-esimo
         * \return retorna el id de la componente conexa
        */
        int findSet( int i );

        /**
         * \fn isSameSet
         * \brief verificar si el i-esimo vertice y el j-esimo vertice pertenecen a la misma componente conexa
         * \param i i-esimo vertice
         * \param j j-esimo vertice
         * \return true si pertenecen a la misma componente de lo contrario false
        */
        bool isSameSet( int i, int j );

        /**
         * \fn unionSet
         * \brief une el i-esimo vertice a la componente conexa a la cual pertenece el j-esimo elemento
         * \param i i-esimo vertice
         * \param j j-esimo vertice
        */
        void unionSet( int i, int j );

        /**
         * \fn distance
         * \brief halla la distancia del vertice p1 al vertice p2
         * \param p1 vertice con indice p1
         * \param p2 vertice con indice p2
         * \return retorna la distancia de p1 a p2
        */
        double distance( id p1, id p2 );

        /** 
         * \fn SlimDow
         * \brief halla la arista cuyo valor sea superior al diametro del MST * 0.55
        */
        bool SlimDow( void );

        /**
         * \fn DFS
         * \brief recorre el grafo contando la cantidad de vertices por componente conexa
         * \param u-esimo vertice del grafo
         * \return retorna la cantidad de vertices por componente conexa
        */
        int DFS( int u );

        /** cantidad de vertices */
        int V;

        /** cantidad de aristas a utilizar  */
        int E;

        /** indice el u-esimo vertice  */
        int u;

        /** indice del v-esimo vertice */
        int v;

        /** peso o distancia entre dos vertices */
        int w;

        /** padre de cada componente conexa */
        vi pset;

        /** cantidad de componentes conexas */
        vi setSize;

        /** grado completo representado por una lista de adyacencia */
        vector<vid> AdjList;

        /** grafo minimun spanning tree */
        vector<vid> AdjListMST;

        /** cola de prioridad de distancias con un par donde almacena los indices de los vertices  */
        priority_queue<id> pq;

        /** diametro del grafo */
        double diameter;

        /** radio del grafo */
        double radio;

        /** identificar los vertices visitados  */
        vector< int > visit;

        /** cantidad de componentes conexas  */
        int _numDisjointSets;
};

#endif // GRAFO_H
