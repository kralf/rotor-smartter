
#include <smart-rotor-interfaces/Messages.h>

#include <rotor/BaseOptions.h>
#include <rotor/Conversion.h>
#include <rotor/FileUtils.h>
#include <rotor/Logger.h>
#include <rotor/NetUtils.h>
#include <rotor/RemoteRegistry.h>
#include <rotor/Time.h>
#include <rotor/Thread.h>

#include "PosePlanner.h"
#include "PoseGraph.h"

#include <sys/time.h>
#include <iostream>
#include <signal.h>

using namespace Rotor;
using namespace std;
using namespace Urus::Wp2;

bool quit = false;

//------------------------------------------------------------------------------

void quitLoop(int q __attribute__((unused))) {
  quit = true;
}

//------------------------------------------------------------------------------

void
mainLoop( Registry & registry, PosePlanner & planner,
  double offsetX, double offsetY )
{
  while ( !quit )
  {
    try
    {
      Message msg = registry.receiveMessage( 1 );

      if ( msg.name() == "planner_plan_message" )
      {
        Structure & data = msg.data();
        planner_plan_message & planMessage =
          * reinterpret_cast<planner_plan_message *>( data.buffer() );

        Logger::spam( "Plan request message has been received:" + data.toString(),
          "pathPlanner" );

        double planStart = seconds();

        PosePlanner::Path path = planner.plan (
          planMessage.start.x + offsetX,
          planMessage.start.y + offsetY,
          planMessage.start.theta,
          planMessage.goal.x + offsetX,
          planMessage.goal.y + offsetY,
          planMessage.goal.theta
        );

        cout << "Generated path of length " << path.size() << " in " <<
          seconds() - planStart << "s" << endl;

        if ( path.size() > 0 )
        {
          Rotor::Structure pathMessage = registry.newStructure("path_message");
          pathMessage["point_count"] = static_cast<int>( path.size() );
          pathMessage.adjust();

          size_t i = 0;
          for ( PosePlanner::Path::const_iterator it = path.begin();
            it != path.end(); ++it, ++i )
          {
            const Pose & pose = *it;

            pathMessage["x"][i]     = pose.x() - offsetX;
            pathMessage["y"][i]     = pose.y() - offsetX;
            pathMessage["theta"][i] = pose.theta();
          }

          pathMessage["timestamp"] = Rotor::seconds();
          pathMessage["host"] = const_cast<char*>( Rotor::hostName().c_str() );
          registry.sendStructure( "path_message", pathMessage );

          Logger::spam( "Path message has been sent:" + pathMessage.toString(),
            "pathPlanner" );
        } else {
          Rotor::Structure stopMessage = registry.newStructure("path_stop_message");

          stopMessage["timestamp"] = Rotor::seconds();
          stopMessage["host"] = const_cast<char*>( Rotor::hostName().c_str() );
          registry.sendStructure( "path_stop_message", stopMessage );

          Logger::spam( "Stop message has been sent:" + stopMessage.toString(),
            "pathPlanner" );
        }
      }
    } catch( MessagingTimeout ) {
      Logger::spam( "Timeout waiting for message", "pathPlanner" );
    }
  }
}

//------------------------------------------------------------------------------

void
registerMessages( Registry & registry )
{
  registry.registerType( ROTOR_DEFINITION_STRING( carmen_point_t ) );

  registry.registerMessageType(
    "planner_plan_message",
    ROTOR_DEFINITION_STRING( planner_plan_message )
  );
  registry.subscribeToMessage( "planner_plan_message", true );

  registry.registerMessageType(
    "path_message",
    ROTOR_DEFINITION_STRING( path_message )
  );

  registry.registerMessageType(
    "path_stop_message",
    ROTOR_DEFINITION_STRING( path_stop_message )
  );
}

//------------------------------------------------------------------------------

int main( int argc, char * argv[] )
{
  if ( argc < 3 )
  {
    cout << "Usage: " << argv[0] << " <costmap.graph> <config.ini>\n";
    exit( 1 );
  }

  string moduleName( argv[0] );

  BaseOptions options;
  options.fromString( fileContents( argv[2] ) );
  RemoteRegistry registry( "CarmenRegistry", moduleName, options, "lib" );

  registerMessages( registry );

  double robotRadius = options.getDouble( "smart", "radius" );

  double offsetX = options.getDouble( moduleName, "offsetX" );
  double offsetY = options.getDouble( moduleName, "offsetY" );

  double originX = options.getDouble( moduleName, "originX" );
  double originY = options.getDouble( moduleName, "originY" );
  double cellSize = options.getDouble( moduleName, "cellSize" );
  double margin = options.getDouble( moduleName, "margin" );
  double arcLength = options.getDouble( moduleName, "arcLength" );
  double maxCurvature = options.getDouble( moduleName, "maxCurvature" );
  size_t branchingFactor = options.getInt( moduleName, "branchingFactor" );

  PosePlanner planner(
    argv[1], cellSize, originX, originY,
    robotRadius, margin,
    arcLength, maxCurvature, branchingFactor
  );

  signal( SIGINT, quitLoop );

  mainLoop( registry, planner, offsetX, offsetY );
}
