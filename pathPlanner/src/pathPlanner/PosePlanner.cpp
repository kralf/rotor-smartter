#include "PosePlanner.h"
#include <rana-graph/AStar.h>
#include <algorithm>
#include <iostream>

using namespace Urus::Wp2;
using namespace Rana::Graph;

//------------------------------------------------------------------------------

double
h( const Pose & p1, const Pose & p2 )
{
  double distance = sqrt( pow( p1.x() - p2.x(), 2 ) + pow( p1.y() - p2.y(), 2 ) );
  return distance;
}

//------------------------------------------------------------------------------

struct G
{
  G( const CostMap & map, double robotRadius, double margin, double arcLength )
    : _map( map ),
    _robotRadius( robotRadius ),
    _margin( margin ),
    _arcLength( arcLength)
  {
  }

  double operator()( const Pose & p1, const Pose & p2 )
  {
    double value = _map( p2.x(), p2.y() );
    if ( value <= _robotRadius ) {
      return -1.0;
    }
    double distance = _arcLength;
    if ( _margin == 0 ) {
      return distance;
    } else {
      return distance * ( 1 + _margin / std::min( _margin, value ) );
    }
  }

  const CostMap & _map;
  double          _robotRadius;
  double          _margin;
  double          _arcLength;
};

//------------------------------------------------------------------------------

bool
termination( const Pose & p1, const Pose & p2 )
{
/*  double thetaDiff = p1.theta() - p2.theta();
  while( thetaDiff > M_PI ) {
    thetaDiff -= 2 * M_PI;
  }
  while( thetaDiff < -M_PI ) {
    thetaDiff += 2 * M_PI;
  }*/
  return     fabs( p1.x() - p2.x() ) < 0.2
          && fabs( p1.y() - p2.y() ) < 0.2;
//           && fabs( thetaDiff ) < 0.4;
}

//------------------------------------------------------------------------------

PosePlanner::PosePlanner(
  std::string fileName,
  double cellSize,
  double x,
  double y,
  double robotRadius,
  double margin,
  double arcLength,
  double maxCurvature,
  double branchingFactor
) : _map( fileName, cellSize, x, y ),
    _graph( arcLength, maxCurvature, branchingFactor ),
    _robotRadius( robotRadius ),
    _margin( margin ),
    _arcLength( arcLength )
{
}

//------------------------------------------------------------------------------

PosePlanner::Path
PosePlanner::plan(
    double x1, double y1, double theta1,
    double x2, double y2, double theta2
)
{
  Path result;
  G g( _map, _robotRadius, _margin, _arcLength );
  aStar( _graph, result, Pose( x1, y1, theta1 ), Pose( x2, y2, theta2 ), g, h, termination );
  return result;
}


//------------------------------------------------------------------------------

const PoseGraph &
PosePlanner::graph() const
{
  return _graph;
}
