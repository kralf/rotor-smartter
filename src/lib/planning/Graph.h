#ifndef RANA_GRAPH_GRAPH_H
#define RANA_GRAPH_GRAPH_H


#include "Exceptions.h"
#include <utility>
#include <set>
#include <map>


template <typename NODE>
class Graph
{
public:
  typedef typename std::set<NODE> Neighborhood;
  typedef typename std::set<NODE> Nodes;
  
  const Neighborhood & children( NODE node ) const throw( NotFoundError );

  const Neighborhood & parents( NODE node ) const throw( NotFoundError );
  
  const Nodes & nodes() const;
  
  bool hasNode( NODE node ) const;
  
  bool hasEdge( NODE node1, NODE node2 ) const;
  
  bool addNode( NODE node );  

  bool addEdge( NODE node1, NODE node2 );
  
  bool removeNode( NODE node );
  
  bool removeEdge( NODE node1, NODE node2 ); 

private:
  typedef typename std::map<NODE, Neighborhood> Neighbors;
  typedef typename std::pair<NODE, NODE> Edge;
  typedef typename std::set<Edge> Edges;
  
  Nodes     _nodes;
  Edges     _edges;
  Neighbors _children;
  Neighbors _parents;
  Nodes     _emptyNodes;
  
};


#include "Graph.tpp"


#endif //RANA_GRAPH_GRAPH_H
