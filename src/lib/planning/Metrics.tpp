#include <cmath>

//------------------------------------------------------------------------------

template <typename NODE>
inline
double 
euclideanDistance( NODE n1, NODE n2 )
{
  return sqrt( ( n1 - n2 ).dot( n1 - n2 ) );
}

//------------------------------------------------------------------------------

template <>
inline
double 
euclideanDistance( int n1, int n2 )
{
  return fabs( n2 - n1 );
}

//------------------------------------------------------------------------------

template <>
inline
double 
euclideanDistance( float n1, float n2 )
{
  return fabs( n2 - n1 );
}

//------------------------------------------------------------------------------

template <>
inline
double 
euclideanDistance( double n1, double n2 )
{
  return fabs( n2 - n1 );
}

//------------------------------------------------------------------------------

template <typename NODE>
bool
equality( NODE n1, NODE n2 )
{
  return n1 == n2;
}
