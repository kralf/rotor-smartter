#include <map>
#include <tr1/unordered_set>
#include <tr1/unordered_map>

#include "PriorityQueue.h"
#include "Metrics.h"

//------------------------------------------------------------------------------

template <typename GRAPH, typename NODE, typename PATH, typename G, typename H, typename TERMINATION >
void 
aStar( const GRAPH & graph, PATH & result, NODE start, NODE goal, G & g, H & h,
  TERMINATION & termination )
{
  typedef std::tr1::unordered_map<NODE, double> Cache;
  typedef std::tr1::unordered_set<NODE> Visited;
  typedef std::tr1::unordered_map<NODE, NODE> Parent;
  
  Cache gCache;
  gCache[start] = 0;
  
  Parent parent;
  Visited visited;
  result.clear();
  
  PriorityQueue<NODE> queue;
  queue.push( start, 0 );
  while ( queue.size() > 0 ) {
    NODE current = queue.pop();
    if ( visited.find( current ) != visited.end() ) {
      continue;
    }
    visited.insert( current );
    if ( termination( current, goal ) ) {
      result.push_front( current );
      typename Parent::iterator it = parent.find( current );
      while ( it != parent.end() ) {
        current = it->second;
        result.push_front( current );
        it = parent.find( current );
      }
      break;
    }
    
    double currentDistance = gCache[current];
    const typename GRAPH::Neighborhood & children = graph.children( current );
    typename GRAPH::Neighborhood::const_iterator childIt;
    typename GRAPH::Neighborhood::const_iterator end = children.end();
    for ( childIt = children.begin(); childIt != end; ++childIt ) {
      if  ( visited.find( *childIt ) == visited.end() ) {
        double gValue = g( current, *childIt );
        if ( gValue >= 0 ) {
          double tmpDistance = currentDistance + gValue;
          typename Cache::iterator cacheIt = gCache.find( *childIt );
          if ( cacheIt == gCache.end() || cacheIt->second > tmpDistance ) {
            gCache[*childIt] = tmpDistance;
            parent[*childIt] = current;
            double aStarCost = tmpDistance + h( goal, *childIt );
            queue.push( *childIt, aStarCost );
          }
        }
      }
    }
  }
}

//------------------------------------------------------------------------------

template <typename GRAPH, typename NODE, typename PATH, typename DISTANCE>
void 
aStar( const GRAPH & graph, PATH & result, NODE start, NODE goal, DISTANCE &
  distance )
{
  aStar( graph, result, start, goal, distance, distance, equality<NODE> );
}

//------------------------------------------------------------------------------

template <typename GRAPH, typename NODE,  typename PATH >
void 
aStar( const GRAPH & graph, PATH & result, NODE start, NODE goal )
{
  aStar( graph, result, start, goal, euclideanDistance<NODE>, euclideanDistance<NODE>, equality<NODE> );
}
