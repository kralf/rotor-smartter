//------------------------------------------------------------------------------

template<typename VALUE>
PriorityQueue<VALUE>::PriorityQueue()
{
}

//------------------------------------------------------------------------------

template<typename VALUE>
void 
PriorityQueue<VALUE>::push( VALUE value, double cost )
{
  _map.insert( std::make_pair( cost, value ) );
}

//------------------------------------------------------------------------------

template<typename VALUE>
VALUE 
PriorityQueue<VALUE>::pop()
{
  VALUE result = _map.begin()->second;
  _map.erase( _map.begin() );
  return result;
}

//------------------------------------------------------------------------------

template<typename VALUE>
size_t 
PriorityQueue<VALUE>::size()
{
  return _map.size();
}
