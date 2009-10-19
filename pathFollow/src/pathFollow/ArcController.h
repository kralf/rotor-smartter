#ifndef ARC_CONTROLLER_H
#define ARC_CONTROLLER_H


#include <utility>
#include <vector>

class Vector;


class ArcController
{
public:
  typedef std::vector<Vector> Path;
  
  ArcController( 
    const Path & path, 
    double axesDistance, 
    double orientationWeight, 
    size_t lookAhead, 
    size_t maxLookAhead, 
    double angleThreshold,
    bool cycle 
  );
    
  double step( const Vector & pose );
  bool finished();

private:
  std::pair<double, double> orientationComponent( const Vector & v1, const Vector & v2 );
  double distanceComponent( const Vector & v1, const Vector & v2 );
  double steeringAngle( const Vector & v1, const Vector & v2 );
  double radiusToSteeringAngle( double radius );
  size_t closest( const Vector & v1 );
  const Vector & waypoint( size_t i );
  const Vector next();

  const Path & _path;
  double       _axesDistance;
  double       _orientationWeight;
  size_t       _lookAhead;
  size_t       _maxLookAhead;
  double       _angleThreshold;
  bool         _cycle;
  size_t       _current;
};


#endif //ARC_CONTROLLER_H
