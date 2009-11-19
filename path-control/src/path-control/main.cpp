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
setupMessages( Registry & registry, const std::string & poseMessage )
{
  registry.registerType( ROTOR_DEFINITION_STRING( carmen_point_t ) );

  if ( poseMessage == "carmen_localize_globalpos" )
  {
    registry.registerMessageType(
      "carmen_localize_globalpos",
      ROTOR_DEFINITION_STRING( carmen_localize_globalpos_message )
    );
    registry.subscribeToMessage( "carmen_localize_globalpos", true );
  } else if ( poseMessage == "carmen_base_odometry" ) {
    registry.registerMessageType(
      "carmen_base_odometry",
      ROTOR_DEFINITION_STRING( carmen_base_odometry_message )
    );
    registry.subscribeToMessage( "carmen_base_odometry", true );
  } else if ( poseMessage == "locfilter_filteredpos_message" ) {
    registry.registerMessageType(
      "locfilter_filteredpos_message",
      ROTOR_DEFINITION_STRING( locfilter_filteredpos_message )
    );
    registry.subscribeToMessage( "locfilter_filteredpos_message", true );
  }

  registry.registerMessageType(
    "path_message",
    ROTOR_DEFINITION_STRING( path_message )
  );
  registry.subscribeToMessage( "path_message", false, 4 );

  registry.registerMessageType(
    "path_stop_message",
    ROTOR_DEFINITION_STRING( path_stop_message )
  );
  registry.subscribeToMessage( "path_stop_message", false, 4 );

  registry.registerMessageType(
    "path_status_message",
    ROTOR_DEFINITION_STRING( path_status_message )
  );
  registry.subscribeToMessage( "path_status_message", false, 4 );

  registry.registerMessageType(
    "planner_plan_message",
    ROTOR_DEFINITION_STRING( planner_plan_message )
  );
}

//------------------------------------------------------------------------------

int
main( int argc, char * argv[] )
{
  string command = "pathControl";
  if ( argc != 3 ) {
    Logger::error( "Usage: " + command + " <map> <config.ini>" );
    exit( 1 );
  }

  BaseOptions options;
  options.fromString( fileContents( argv[2] ) );
  RemoteRegistry registry( "CarmenRegistry", "pathControl", options, "lib" );

  string poseMessage = options.getString( "pathControl", "poseMessage" );
  setupMessages( registry, poseMessage );

  QApplication application( argc, argv );
  ApplicationWindow * window = new ApplicationWindow();

  window->mainWidget().pathPlot->setRegistry( registry );
  window->mainWidget().pathPlot->setMap( argv[1] );

  DispatchThread dispatch( registry, *window );

  dispatch.start();
  window->show();
  exit( application.exec() );
}
