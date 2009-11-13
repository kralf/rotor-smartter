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
    double securityDistance,
    double width
  );

  double step( double velocity, double steeringAngle,
    const std::vector<double> & laserX, const std::vector<double> & laserY );
private:
  double       _axesDistance;
  double       _laserDistance;
  double       _securityDistance;
  double       _width;
  double       _offset;
};


#endif //ARC_SAFETY_H
