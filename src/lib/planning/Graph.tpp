//------------------------------------------------------------------------------

template <typename NODE>
const typename Graph<NODE>::Neighborhood &
Graph<NODE>::children( NODE node ) const
throw( NotFoundError )
{
  typename Neighbors::const_iterator it = _children.find( node );
  if ( it == _children.end() ) {
    throw NotFoundError( "Node is not in the graph" );
  }
  return it->second;
}
  
//------------------------------------------------------------------------------

template <typename NODE>
const typename Graph<NODE>::Neighborhood &
Graph<NODE>::parents( NODE node ) const
throw( NotFoundError )
{
  typename Neighbors::const_iterator it = _parents.find( node );
  if ( it == _parents.end() ) {
    throw NotFoundError( "Node is not in the graph" );
  }
  return it->second;
}

//------------------------------------------------------------------------------

template <typename NODE>
const typename Graph<NODE>::Nodes &
Graph<NODE>::nodes() const
{
  return _nodes;
}

//------------------------------------------------------------------------------

template <typename NODE>
bool 
Graph<NODE>::hasNode( NODE node ) const
{
  return _nodes.find( node ) != _nodes.end();
}

//------------------------------------------------------------------------------

template <typename NODE>
bool 
Graph<NODE>::hasEdge( NODE node1, NODE node2 ) const
{
  return _edges.find( std::make_pair( node1, node2 ) ) != _edges.end();
}
//------------------------------------------------------------------------------

template <typename NODE>
bool 
Graph<NODE>::addNode( NODE node )
{
  _children[node];
  _parents[node];
  return _nodes.insert( node ).second;
}  

//------------------------------------------------------------------------------

template <typename NODE>
bool 
Graph<NODE>::addEdge( NODE node1, NODE node2 )
{    
  Nodes & children = _children[node1];
  if ( children.find( node2 ) != children.end() )
    return false;

  _nodes.insert( node1 );
  _nodes.insert( node2);

  children.insert( node2 ); 
  _parents[node2].insert( node1 );

  _children[node2];
  _parents[node1];
  _edges.insert( std::make_pair( node1, node2 ) );
  return true;
}

//------------------------------------------------------------------------------

template <typename NODE>
bool 
Graph<NODE>::removeNode( NODE node ) {
  if ( _nodes.erase( node ) ) {
    for ( 
      typename Nodes::iterator i = _children[node].begin();
      i != _children[node].end();
      i++
    ) {
      _edges.erase( std::make_pair( node, *i ) );
      _parents[*i].erase( node );
    }
    _children.erase( node );
    
    
    for ( 
      typename Nodes::iterator i = _parents[node].begin();
      i != _parents[node].end();
      i++
    ) {
      _edges.erase( std::make_pair( *i, node ) );
      _children[*i].erase( node );
    }
    _parents.erase( node );
    
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------

template <typename NODE>
bool 
Graph<NODE>::removeEdge( NODE node1, NODE node2 )
{
  if ( _edges.erase( std::make_pair( node1, node2 ) ) ) {
    _children[node1].erase( node2 );
    _parents[node2].erase( node1 );
    return true;
  }
  return false;
}


