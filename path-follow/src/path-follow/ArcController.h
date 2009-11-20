#ifndef ARC_CONTROLLER_H
#define ARC_CONTROLLER_H


#include "Types.h"
#include <utility>
#include <vector>


class ArcController
{
public:
  ArcController(
    double axesDistance,
    double orientationWeight,
    size_t lookAhead,
    size_t maxLookAhead,
    double lookAheadThreshold,
    double velocity,
    double deceleration,
    bool cycle
  );

  void path ( const Path & path );
  const Point& current () const;

  double radiusToSteeringAngle( double radius );
  double steeringAngleToRadius( double angle );

  std::pair<double, double> step( const Vector & pose );
  bool finished();

private:
  std::pair<double, double> orientationComponent( const Vector & v1, const Vector & v2 );
  double distanceComponent( const Vector & v1, const Vector & v2 );
  double steeringAngle( const Vector & v1, const Vector & v2 );
  double velocity( const Vector & v1, const Vector & v2 );
  size_t closest( const Vector & v1 );
  const Vector & waypoint( size_t i );
  const Vector next( const Vector & pose );

  Path         _path;
  double       _axesDistance;
  double       _orientationWeight;
  size_t       _lookAhead;
  size_t       _maxLookAhead;
  double       _lookAheadThreshold;
  double       _velocity;
  double       _deceleration;
  bool         _cycle;
  size_t       _current;
};


#endif //ARC_CONTROLLER_H
