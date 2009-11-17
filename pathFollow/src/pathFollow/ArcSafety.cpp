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
  double securityDeceleration,
  double securityMinDistance,
  size_t securityMinHits,
  double width
)
  : _axesDistance( axesDistance ),
    _laserDistance( laserDistance ),
    _securityDeceleration( securityDeceleration ),
    _securityMinDistance( securityMinDistance ),
    _securityMinHits( securityMinHits ),
    _width( width ),
    _offset( width / 2.0 )
{
}

//------------------------------------------------------------------------------

double
ArcSafety::step( double velocity, double steeringAngle,
  const vector<double> & laserX, const vector<double> & laserY,
  const vector<unsigned char> & laserStatus )
{
  size_t numPoints = 0;

  double sign   = 0;
  double radius = 0;
  if ( fabs( steeringAngle ) > 1E-6 ) {
    sign   = -steeringAngle / fabs( steeringAngle );
    radius = sign * sqrt( sqr( _axesDistance / sin( steeringAngle ) ) -
      sqr( _axesDistance ) );
  } else {
    radius = 4000.0;
  }

  double r1 = sqrt( sqr( abs( radius ) - _offset ) + sqr( _laserDistance ) );
  double r2 = sqrt( sqr( abs( radius ) + _offset ) + sqr( _laserDistance ) );
  double d_max = sqr( velocity ) / _securityDeceleration;
  double ds_min = d_max;

  for ( size_t i = 0; i < laserX.size(); ++i )
  {
    if ( !laserStatus[i]) {
      double r = sqrt( sqr( radius + laserY[i] ) + sqr( laserX[i] +
        _laserDistance ) );
      double a = atan2( laserX[i] + _laserDistance, -radius - laserY[i] );
      double d = fabs( r * a );

      if ( ( r >= r1 ) && ( r <= r2 ) )
      {
        if ( d <= _securityMinDistance )
        {
          ++numPoints;
          if ( numPoints > _securityMinHits )
            return 0.0;
        } else {
          if ( d - _securityMinDistance < ds_min )
            ds_min = d - _securityMinDistance;
        }
      }
    }
  }

  if ( ds_min < d_max )
    return sqrt( ( ds_min - _securityMinDistance ) * _securityDeceleration );
  else
    return velocity;
}
