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
