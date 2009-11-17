#ifndef ARC_SAFETY_H
#define ARC_SAFETY_H

#include <vector>

#include "Types.h"


class ArcSafety
{
public:
  ArcSafety(
    double axesDistance,
    double laserDistance,
    double securityDeceleration,
    double securityMinDistance,
    size_t securityMinHits,
    double width
  );

  double step( double velocity, double steeringAngle,
    const std::vector<double> & laserX, const std::vector<double> & laserY,
    const std::vector<unsigned char> & laserStatus );
private:
  double       _axesDistance;
  double       _laserDistance;
  double       _securityDeceleration;
  double       _securityMinDistance;
  size_t       _securityMinHits;
  double       _width;
  double       _offset;
};


#endif //ARC_SAFETY_H
