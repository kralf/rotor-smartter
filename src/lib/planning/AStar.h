#ifndef RANA_GRAPH_ASTAR_H
#define RANA_GRAPH_ASTAR_H


#include "Graph.h"


template <typename GRAPH, typename NODE, typename PATH, typename G, typename H,
typename TERMINATION >
void aStar( const GRAPH & graph, PATH & result, NODE start, NODE goal, G & g, H & h, TERMINATION & termination );

template <typename GRAPH, typename NODE, typename PATH, typename DISTANCE>
void aStar( const GRAPH & graph, PATH & result, NODE start, NODE goal, DISTANCE & distanceF );

template <typename GRAPH, typename NODE,  typename PATH >
void aStar( const GRAPH & graph, PATH & result, NODE start, NODE goal );


#include "AStar.tpp"


#endif //RANA_GRAPH_ASTAR_H
