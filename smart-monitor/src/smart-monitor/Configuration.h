#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include <rotor/Options.h>
#include <string>


class Configuration
{
public:
  Configuration( 
    const std::string & moduleName,
    const Rotor::Options & options );
  double scale();
  double wheelDistance();
  double axesDistance();
  double laserDistance();
  double securityDistance();
  
private:
  std::string _platform;
  double      _scale;
  double      _wheelDistance;
  double      _axesDistance;
  double      _laserDistance;
  double      _securityDistance;
};


#endif //CONFIGURATION_H
