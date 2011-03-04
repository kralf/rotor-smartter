#include "Configuration.h"
#include <rotor/Options.h>

using namespace Rotor;
using namespace std;


//------------------------------------------------------------------------------

Configuration::Configuration( 
  const string & moduleName, 
  const Options & options 
)
  : _platform( options.getString( moduleName, "platform" ) ),
    _scale( options.getDouble( moduleName, "scale" ) ),
    _defaultPath( options.getString( moduleName, "defaultPath" ) ),
    _setGoalProcess( options.getString( moduleName, "setGoalProcess" ) ),
    _setPoseProcess( options.getString( moduleName, "setPoseProcess" ) ),
    _wheelDistance( options.getDouble( "smart", "wheelDistance" ) ),
    _axesDistance( options.getDouble( "smart", "axesDistance" ) ),
    _laserDistance( options.getDouble( "smart", "laserDistance" ) ),
    _securityDistance( options.getDouble( "smart", "securityDistance" ) )
{
}

//------------------------------------------------------------------------------

double
Configuration::scale() const
{
  return _scale;
}

//------------------------------------------------------------------------------

const std::string &
Configuration::defaultPath() const
{
  return _defaultPath;
}

//------------------------------------------------------------------------------

const std::string &
Configuration::setGoalProcess() const
{
  return _setGoalProcess;
}

//------------------------------------------------------------------------------

const std::string &
Configuration::setPoseProcess() const
{
  return _setPoseProcess;
}

//------------------------------------------------------------------------------

double
Configuration::wheelDistance() const
{
  return _wheelDistance;
}

//------------------------------------------------------------------------------

double
Configuration::axesDistance() const
{
  return _axesDistance;
}

//------------------------------------------------------------------------------

double
Configuration::laserDistance() const
{
  return _laserDistance;
}

//------------------------------------------------------------------------------

double
Configuration::securityDistance() const
{
  return _securityDistance;
}
