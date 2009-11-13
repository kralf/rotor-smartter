#include "ArcController.h"
#include "Vector.h"
#include <rotor/Logger.h>
#include <rotor/Conversion.h>
#include <cfloat>

using namespace std;
using namespace Rotor;

//------------------------------------------------------------------------------

double shortestAngle( double angle )
{
  while ( angle > M_PI )
  {
    angle = angle - M_PI;
  }
  while ( angle < -M_PI )
  {
    angle = angle + M_PI;
  }
  return angle;
}

//------------------------------------------------------------------------------

ArcController::ArcController(
  double axesDistance,
  double orientationWeight,
  size_t lookAhead,
  size_t maxLookAhead,
  double angleThreshold,
  bool cycle
)
  : _axesDistance( axesDistance ),
    _orientationWeight( orientationWeight ),
    _lookAhead( lookAhead ),
    _maxLookAhead( maxLookAhead ),
    _angleThreshold( angleThreshold ),
    _cycle( cycle ),
    _current( 0 )
{
}

//------------------------------------------------------------------------------

void
ArcController::path ( const Path & path )
{
  _path = path;
  _current = 0;
}

//------------------------------------------------------------------------------

double
ArcController::step( const Vector & pose )
{
  _current = closest( pose );
  if ( pose.distance( _path.back().origin() ) < 0.1 && ! _cycle )
  {
    _current = _path.size() - 1;
  }
  return steeringAngle( pose, next() );
}

//------------------------------------------------------------------------------

bool
ArcController::finished()
{
  return ! ( _current < _path.size() ) && ! _cycle;
}

//------------------------------------------------------------------------------

pair<double, double>
ArcController::orientationComponent( const Vector & v1, const Vector & v2 )
{
  Point v3      = v1.orthogonal().intersection( v2.orthogonal() );
  double radius = v1.radius( v3 );
  double error  = fabs( v2.distance( v3 ) - v1.distance( v3 ) );
  return make_pair( error, radius );
}

//------------------------------------------------------------------------------

double
ArcController::distanceComponent( const Vector & v1, const Vector & v2 )
{
  Point origin    = ( v1.origin() + v2.origin() ) / 2.0;
  Point direction = v2.origin() - v1.origin();
  Vector v3 = Vector( origin, direction );
  Point v4 = v3.orthogonal().intersection( v1.orthogonal() );
  double radius = v1.radius( v4 );
  return radius;
}

//------------------------------------------------------------------------------

double
ArcController::steeringAngle( const Vector & v1, const Vector & v2 )
{
  pair<double, double> tmp = orientationComponent( v1, v2 );
  double t1 = tmp.second;
  double t2 = distanceComponent( v1, v2 );
  double a1 = radiusToSteeringAngle( t1 );
  double a2 = radiusToSteeringAngle( t2 );
  double w1 = _orientationWeight;
  double w2 = tmp.first;
  double angle = ( w1 * a1 + w2 * a2 ) / ( w1 + w2 );
  return angle;
}

//------------------------------------------------------------------------------

size_t
ArcController::closest( const Vector & v1 )
{
  size_t index = 0;
  double distance = DBL_MAX;
  for ( size_t i = _current; i < _path.size(); ++i )
  {
    const Vector & v   = _path[i];
    double tmpDistance = v.distance( v1.origin() );
    if ( tmpDistance < distance )
    {
      index    = i;
      distance = tmpDistance;
    }
  }
  return index;
}

//------------------------------------------------------------------------------

double
ArcController::radiusToSteeringAngle( double radius )
{
  int sign = radius / fabs( radius );
  return sign * shortestAngle( atan2( _axesDistance, fabs( radius ) ) );
}

//------------------------------------------------------------------------------

double
ArcController::steeringAngleToRadius( double angle )
{
  int sign = -angle / fabs( angle );
  return sign * sqrt( pow( _axesDistance / sin( angle ), 2 ) - pow( _axesDistance, 2 ) );
}

//------------------------------------------------------------------------------

const Vector &
ArcController::waypoint( size_t i )
{
  return _path[i % _path.size()];
}

//------------------------------------------------------------------------------

const Vector
ArcController::next()
{
  const Vector & current = waypoint( _current );
  double angle           = current.angle();
  Vector next            = _path.back();
  for ( size_t i = _lookAhead; i <= _maxLookAhead; ++i )
  {
    if ( ! _cycle && _current + i >= _path.size() )
    {
      break;
    }
    const Vector & tmpNext = waypoint( _current + i );
    if (   i == _lookAhead
        || fabs( shortestAngle( angle - tmpNext.angle() ) ) < _angleThreshold )
    {
      next = tmpNext;
    }
  }
  return next;
}
