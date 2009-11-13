#include "ArcSafety.h"

using namespace std;

//------------------------------------------------------------------------------

double sqr( double value )
{
  return value * value;
}

//------------------------------------------------------------------------------

ArcSafety::ArcSafety(
  double axesDistance,
  double laserDistance,
  double securityDistance,
  double width
)
  : _axesDistance( axesDistance ),
    _laserDistance( laserDistance ),
    _securityDistance( securityDistance ),
    _width( width ),
    _offset( width / 2.0 )
{
}

//------------------------------------------------------------------------------

double
ArcSafety::step( double velocity, double steeringAngle,
  const vector<double> & laserX, const vector<double> & laserY )
{
  double radius = 400.0;

  if ( steeringAngle != 0 )
  {
    double sign = -steeringAngle / abs( steeringAngle );
    radius = sign * sqrt( sqr( _axesDistance / sin( steeringAngle ) ) -
      sqr( _axesDistance ) );
  }

  double r1 = sqrt( sqr( abs( radius ) - _offset ) + sqr( _laserDistance ) );
  double r2 = sqrt( sqr( abs( radius ) + _offset ) + sqr( _laserDistance ) );
  double a1 = atan2( _laserDistance, -radius + _offset );
  double a2 = atan2( _laserDistance, -radius - _offset );

  if ( radius < 0 )
    a2 += _securityDistance / r1;
  else
    a1 -= _securityDistance / r1;

  for ( size_t i = 0; i < laserX.size(); ++i )
  {
    double r  = sqrt( sqr( radius + laserY[i] ) + sqr( laserX[i] + _laserDistance ) );
    if ( ( r >= r1 ) && ( r <= r2 ) )
    {
      double a  = atan2( laserX[i] + _laserDistance, -radius - laserY[i] );
      if ( ( a >= a1 ) && ( a <= a2 ) )
      {
        velocity = 0.0;
      }
    }
  }

  return velocity;
}
