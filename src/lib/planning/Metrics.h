#ifndef RANA_GRAPH_METRICS_H
#define RANA_GRAPH_METRICS_H


template <typename NODE>
double euclideanDistance( NODE n1, NODE n2 );

template <>
double euclideanDistance( int n1, int n2 );

template <>
double euclideanDistance( float n1, float n2 );

template <>
double euclideanDistance( double n1, double n2 );

template <typename NODE>
bool equality( NODE n1, NODE n2 );


#include "Metrics.tpp"


#endif //RANA_GRAPH_METRICS_H
