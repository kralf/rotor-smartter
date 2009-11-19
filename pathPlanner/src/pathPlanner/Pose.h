#ifndef URUS_WP2_POSE_H
#define URUS_WP2_POSE_H


#include <cstdlib>
#include <tr1/unordered_map>

namespace Urus {
namespace Wp2 {


class Pose
{
public:
  Pose();
  Pose( double x, double y, double theta );
  double x() const;
  double y() const;
  double theta() const;
  void x( double x );
  void y( double y );
  void theta( double value );
  bool operator==( const Pose & other ) const;
  bool operator!=( const Pose & other ) const;
  bool operator<( const Pose & other ) const;
  static void distanceTolerance( double value );
  static void angleTolerance( double value );
  Pose next( double distance, double curvature ) const;

private:
  static double _distanceTolerance;
  static double _angleTolerance;
  double _x;
  double _y;
  double _theta;
};


}
}

namespace std {
namespace tr1 {

template<>
struct hash<Urus::Wp2::Pose>
{
  size_t operator()( const Urus::Wp2::Pose & value ) const
  {
    size_t seed =   static_cast<size_t>( value.x() )
                  + 0x9e3779b9;
    seed       ^=   static_cast<size_t>( value.y() )
                  + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
    seed       ^=   static_cast<size_t>( value.theta() )
                  + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
    return seed;
  }
};

}
}

#include "Pose-inline.h"


#endif //URUS_WP2_POSE_H
