#ifndef URUS_WP2_COST_MAP_H
#define URUS_WP2_COST_MAP_H


#include <string>


struct CvMat;


namespace Urus {
namespace Wp2 {


class CostMap
{
public:
  CostMap( const std::string & fileName, double cellSize, double x, double y );
  
  virtual ~CostMap();

  double operator()( double x, double y ) const;
  
private:
  double   _cellSize;
  double   _x;
  double   _y;
  CvMat  * _map;
};


}
}


#include "CostMap-inline.h"


#endif //URUS_WP2_COST_MAP_H
