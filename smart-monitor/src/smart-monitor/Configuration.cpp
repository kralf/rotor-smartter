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
Configuration::scale()
{
  return _scale;
}

//------------------------------------------------------------------------------

double
Configuration::wheelDistance()
{
  return _wheelDistance;
}

//------------------------------------------------------------------------------

double
Configuration::axesDistance()
{
  return _axesDistance;
}

//------------------------------------------------------------------------------

double
Configuration::laserDistance()
{
  return _laserDistance;
}

//------------------------------------------------------------------------------

double
Configuration::securityDistance()
{
  return _securityDistance;
}
