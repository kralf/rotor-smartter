#include "CostMap.h"

using namespace std;

//------------------------------------------------------------------------------

CostMap::CostMap( const string & fileName, double cellSize, double x, double y )
  : _cellSize( cellSize ),
    _x( x ),
    _y( y )
{
  _map = static_cast<CvMat*>( cvLoad( fileName.c_str() ) );
}

//------------------------------------------------------------------------------

CostMap::~CostMap()
{
  cvReleaseMat( &_map );
}
