#include "Vector.h"

//------------------------------------------------------------------------------

Vector::Vector()
{
}

//------------------------------------------------------------------------------

Vector::Vector( const Point & origin, const Point & direction )
  : _origin( origin ),
    _direction( direction )
{
  normalize();
}

//------------------------------------------------------------------------------

Vector::Vector( const Point & origin, double direction )
  : _origin( origin )
{
  _direction[0] = cos( direction );
  _direction[1] = sin( direction );
}

//------------------------------------------------------------------------------

Point &
Vector::origin()
{
  return _origin;
}

//------------------------------------------------------------------------------

const Point &
Vector::origin() const
{
  return _origin;
}

//------------------------------------------------------------------------------

void
Vector::origin( const Point & value )
{
  _origin = value;
}

//------------------------------------------------------------------------------

Point &
Vector::direction()
{
  return _direction;
}

//------------------------------------------------------------------------------

const Point &
Vector::direction() const
{
  return _direction;
}

//------------------------------------------------------------------------------

void
Vector::direction( const Point & value )
{
  _direction = value;
}

//------------------------------------------------------------------------------

void
Vector::normalize()
{
  double length = sqrt( _direction.dot( _direction )  );
  _direction    = _direction / length;
}

//------------------------------------------------------------------------------

double
Vector::distance( const Point & other ) const
{
  Point difference = other - _origin;
  return sqrt( difference.dot( difference ) );
}

//------------------------------------------------------------------------------

Point
Vector::projection( const Point & point ) const
{
  Point w     = point - _origin;
  double vsq  = _direction.dot( _direction );
  double proj = w.dot( _direction );
  return _origin + proj * _direction / vsq;
}

//------------------------------------------------------------------------------

double
Vector::angle() const
{
  return atan2( _direction[1], _direction[2] );
}

//------------------------------------------------------------------------------

bool
Vector::parallel( const Vector & other ) const
{
  return 1.0 - fabs( other._direction.dot( _direction ) ) <= 0;
}

//------------------------------------------------------------------------------

double
Vector::alignment( const Vector & other ) const
{
  return other._direction.dot( _direction );
}

//------------------------------------------------------------------------------

Vector
Vector::extrapolate( double amount ) const
{
  return Vector( _origin + amount * _direction, _direction );
}

//------------------------------------------------------------------------------

Point
Vector::intersection( const Vector & other ) const
{
  double x0 = _origin[0];
  double y0 = _origin[1];
  double x1 = _direction[0];
  double y1 = _direction[1];
  double x2 = other._origin[0];
  double y2 = other._origin[1];
  double x3 = other._direction[0];
  double y3 = other._direction[1];
  double denominator = ( x1 * y3 - x3 * y1 );
  double b = ( x1 * y0 + x2 * y1 - x0 * y1 - x1 * y2 ) / denominator;
  return Point(
    other._origin[0] + b * other._direction[0],
    other._origin[1] + b * other._direction[1]
  );
}

//------------------------------------------------------------------------------

Vector
Vector::orthogonal() const
{
  return Vector( _origin, Point( -_direction[1], _direction[0] ) );
}

//------------------------------------------------------------------------------

double 
Vector::radius( const Point & center ) const
{
  const Point & p1 = center;
  const Point & p2 = _origin;
  const Point & p3 = extrapolate( 1 )._origin;
  double sign =   ( p2[0] - p1[0] ) * ( p3[1] - p1[1] ) 
                - ( p2[1] - p1[1] ) * ( p3[0] - p1[0] );
  double r = distance( center );
  if ( sign >= 0 )
  {
    return r;
  } else {
    return -r;
  }
}
