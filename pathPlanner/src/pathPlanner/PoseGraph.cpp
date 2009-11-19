#include "PoseGraph.h"
#include <opencv/cv.h>
#include <iostream>

using namespace Urus::Wp2;
using namespace Rana::Graph;
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

//   Nodes & nodes = const_cast<Nodes&>( _nodes );

  for (
    double curvature = -_maxCurvature;
    curvature <= _maxCurvature;
    curvature += _increment
  )
  {
    children.push_back( child( node, curvature ) );
//     Pose & pose = children.back();
//     nodes.insert( pose );
  }
  return _children;
}

//------------------------------------------------------------------------------

Pose
PoseGraph::child( const Pose & node, double curvature ) const
{
//   fprintf( stderr, "Parent: x=%f, y=%f\n", node.x(), node.y() );
//   fprintf( stderr, "Curvature: %f\n", curvature );
  Pose result = node.next( _arcLength, curvature  );
//   fprintf( stderr, "Son: x=%f, y=%f\n", result.x(), result.y() );
  return result;
/*  return Pose(
    node.x() - radius * sin( node.theta() ) + radius * sin( node.theta() + deltaTheta ),
    node.y() + radius * cos( node.theta() ) - radius * cos( node.theta() + deltaTheta ),
    node.theta() + deltaTheta
  );*/
}


//------------------------------------------------------------------------------

const PoseGraph::Nodes &
PoseGraph::nodes() const
{
  return _nodes;
}
