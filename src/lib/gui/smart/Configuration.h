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
  double scale() const;
  const std::string & defaultPath() const;
  const std::string & setGoalProcess() const;
  const std::string & setPoseProcess() const;
  double wheelDistance() const;
  double axesDistance() const;
  double laserDistance() const;
  double securityDistance() const;
  
private:
  std::string _platform;
  double      _scale;
  std::string _defaultPath;
  std::string _setGoalProcess;
  std::string _setPoseProcess;
  double      _wheelDistance;
  double      _axesDistance;
  double      _laserDistance;
  double      _securityDistance;
};


#endif //CONFIGURATION_H
