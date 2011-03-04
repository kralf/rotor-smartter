#include <iostream>

#include "PoseGraph.h"

using namespace std;

//------------------------------------------------------------------------------

PoseGraph::PoseGraph(
  double arcLength,
  double maxCurvature,
  size_t branchingFactor )
  : _arcLength( arcLength ),
    _maxCurvature( maxCurvature ),
    _increment( _maxCurvature * 2.0 / ( branchingFactor - 1 ) )
{
}

//------------------------------------------------------------------------------

const PoseGraph::Neighborhood &
PoseGraph::children( const Pose & node ) const throw( NotFoundError )
{
  Neighborhood & children = const_cast<Neighborhood&>( _children );
  children.clear();

  for (
    double curvature = -_maxCurvature;
    curvature <= _maxCurvature;
    curvature += _increment
  )
  {
    children.push_back( child( node, curvature ) );
  }
  return _children;
}

//------------------------------------------------------------------------------

Pose
PoseGraph::child( const Pose & node, double curvature ) const
{
  Pose result = node.next( _arcLength, curvature  );
  return result;
}


//------------------------------------------------------------------------------

const PoseGraph::Nodes &
PoseGraph::nodes() const
{
  return _nodes;
}
