#ifndef RANA_GRAPH_PRIORITY_QUEUE_H
#define RANA_GRAPH_PRIORITY_QUEUE_H


#include <map>
#include <tr1/unordered_set>


template< typename VALUE >
class PriorityQueue {
public:
  PriorityQueue();
  void push( VALUE value, double cost );
  VALUE pop();
  size_t size();
private:
  std::multimap<double, VALUE>   _map;
  std::tr1::unordered_set<VALUE> _values;
};


#include "PriorityQueue.tpp"


#endif //RANA_GRAPH_PRIORITY_QUEUE_H
