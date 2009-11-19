#include "Pose.h"

using namespace Urus::Wp2;

double Pose::_distanceTolerance = 0.2;
double Pose::_angleTolerance    = 0.05;

//------------------------------------------------------------------------------

Pose::Pose()
  : _x( 0.0 ),
    _y( 0.0 ),
    _theta( 0.0 )
{
}

//------------------------------------------------------------------------------

Pose::Pose( double x, double y, double theta )
  : _x( x ),
    _y( y ),
    _theta( theta )
{
}


//------------------------------------------------------------------------------

void
Pose::distanceTolerance( double value )
{
  _distanceTolerance = value;
}

//------------------------------------------------------------------------------

void
Pose::angleTolerance( double value )
{
  _angleTolerance = value;
}
