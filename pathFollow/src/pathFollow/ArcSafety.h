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
    double thresholdDistance,
    double securityDistance,
    double width
  );

  double step( double velocity, double steeringAngle,
    const std::vector<double> & laserX, const std::vector<double> & laserY,
    const std::vector<unsigned char> & laserStatus );
private:
  double       _axesDistance;
  double       _laserDistance;
  double       _thresholdDistance;
  double       _securityDistance;
  double       _width;
  double       _offset;
};


#endif //ARC_SAFETY_H
