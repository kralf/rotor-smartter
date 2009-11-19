#include <cmath>

//------------------------------------------------------------------------------

inline
double
Urus::Wp2::Pose::x() const
{
  return _x;
}

//------------------------------------------------------------------------------

inline
double
Urus::Wp2::Pose::y() const
{
  return _y;
}

//------------------------------------------------------------------------------

inline
double
Urus::Wp2::Pose::theta() const
{
  return _theta;
}

//------------------------------------------------------------------------------

inline
void
Urus::Wp2::Pose::x( double x )
{
  _x = x;
}

//------------------------------------------------------------------------------

inline
void
Urus::Wp2::Pose::y( double y )
{
  _y = y;
}

//------------------------------------------------------------------------------

inline
void
Urus::Wp2::Pose::theta( double value )
{
  _theta = value;
}

//------------------------------------------------------------------------------

inline
bool
Urus::Wp2::Pose::operator==( const Pose & other ) const
{
//   return _x == other._x && _y == other._y && _theta == other._theta;
  double xDiff = _x - other._x;
  double yDiff = _y - other._y;
  if ( ! (    xDiff > -_distanceTolerance && xDiff < _distanceTolerance
           && yDiff > -_distanceTolerance && yDiff < _distanceTolerance )
  )
  {
    return false;
  }

  double thetaDiff = _theta - other._theta;
  while( thetaDiff > M_PI ) {
    thetaDiff -= 2 * M_PI;
  }
  while( thetaDiff < -M_PI ) {
    thetaDiff += 2 * M_PI;
  }
  return thetaDiff > -_angleTolerance && thetaDiff < _angleTolerance;

}

//------------------------------------------------------------------------------

inline
bool
Urus::Wp2::Pose::operator!=( const Pose & other ) const
{
  return ! operator!=( other );
}

//------------------------------------------------------------------------------

inline
bool
Urus::Wp2::Pose::operator<( const Pose & other ) const {
  if ( _x < other._x ) {
    return true;
  } else if ( _x == other._x ) {
    if ( _y < other._y ) {
      return true;
    } else if ( _y == other._y ) {
      return _theta < other._theta;
    }
  }
  return false;
}

//------------------------------------------------------------------------------

inline
Urus::Wp2::Pose
Urus::Wp2::Pose::next( double length, double curvature ) const {
  double radius = 1E6;
  if ( curvature != 0 ) {
    radius = 1.0 / curvature;
  }
  double deltaTheta = length / radius;
  return Pose(
    _x - radius * sin( _theta ) + radius * sin( _theta + deltaTheta ),
    _y + radius * cos( _theta ) - radius * cos( _theta + deltaTheta ),
    _theta + deltaTheta
  );
}
