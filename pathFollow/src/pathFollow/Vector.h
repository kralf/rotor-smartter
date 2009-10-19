#ifndef VECTOR_H
#define VECTOR_H


#include "Types.h"


class Vector
{
public:
  Vector();
  Vector( const Point & origin, const Point & direction );
  Vector( const Point & origin, double direction );
  Point & origin();
  const Point & origin() const;
  void origin( const Point & value );
  Point & direction();
  const Point & direction() const;
  void direction( const Point & value );
  void normalize();
  double distance( const Point & other ) const;
  Point projection( const Point & point ) const;
  double angle() const;
  bool parallel( const Vector & other ) const;
  double alignment( const Vector & other ) const;
  Vector extrapolate( double amount ) const;
  Point intersection( const Vector & other ) const ;
  Vector orthogonal() const;
  double radius( const Point & center ) const;
  
  
private:
  Point _origin;
  Point _direction;
};


#endif //VECTOR_H
