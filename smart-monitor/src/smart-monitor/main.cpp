#include "Configuration.h"
#include "DispatchThread.h"
#include "ApplicationWindow.h"

#include <smart-rotor-interfaces/Messages.h>

#include <QApplication>

#include <rotor/Rotor.h>
#include <rotor/BaseOptions.h>
#include <rotor/Structure.h>
#include <rotor/RemoteRegistry.h>
#include <rotor/Logger.h>
#include <rotor/FileUtils.h>

#include <string>
#include <cstdlib>


using namespace Rotor;
using namespace std;


//------------------------------------------------------------------------------

void
setupMessages( Registry & registry )
{
  registry.registerMessageType( 
    "axt_message", 
    ROTOR_DEFINITION_STRING( axt_message ) 
  );
  registry.subscribeToMessage( "axt_message", false, 4 );
  
  registry.registerType( ROTOR_DEFINITION_STRING( carmen_point_t ) );
  
  registry.registerMessageType( 
    "carmen_localize_globalpos", 
    ROTOR_DEFINITION_STRING( carmen_localize_globalpos_message ) 
  );
  registry.subscribeToMessage( "carmen_localize_globalpos", false, 4 );

  registry.registerMessageType( 
    "carmen_base_odometry", 
    ROTOR_DEFINITION_STRING( carmen_base_odometry_message ) 
  );
  registry.subscribeToMessage( "carmen_base_odometry", false, 4 );
}


//------------------------------------------------------------------------------

int
main( int argc, char * argv[] )
{
  string command = "smartMonitor";
  if ( argc != 2 ) {
    Logger::error( "Usage: " + command + " <config.ini>" );
    exit( 1 );
  }
  
  BaseOptions options;
  options.fromString( fileContents( argv[1] ) );
  RemoteRegistry registry( "CarmenRegistry", "monitor", options, "lib" );
  Configuration configuration( command, options );
  
  setupMessages( registry );
  
  QApplication application( argc, argv );
  ApplicationWindow * window = new ApplicationWindow( configuration );
  DispatchThread dispatch( registry, *window );
  
  dispatch.start();
  window->show();
  exit( application.exec() );
}
