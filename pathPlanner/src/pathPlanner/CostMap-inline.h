#include <opencv/cv.h>

//------------------------------------------------------------------------------

inline
double
Urus::Wp2::CostMap::operator()( double x, double y ) const
{
  int iX = static_cast<int>(  x / _cellSize + _x );
  int iY = static_cast<int>( -y / _cellSize + _y );
  if ( iY < 0 || iY >= _map->height || iX < 0 || iX >= _map->width ) {
    return 0;
  } else {
    return CV_MAT_ELEM( *_map, float, iY, iX );
  }
}
