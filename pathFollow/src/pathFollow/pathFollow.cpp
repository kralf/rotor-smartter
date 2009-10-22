#include "ArcController.h"
#include "Vector.h"
#include <smart-rotor-interfaces/Messages.h>
#include <rotor/BaseOptions.h>
#include <rotor/Conversion.h>
#include <rotor/FileUtils.h>
#include <rotor/Logger.h>
#include <rotor/NetUtils.h>
#include <rotor/RemoteRegistry.h>
#include <rotor/Time.h>
#include <string>
#include <sstream>
#include <fstream>  

using namespace std;
using namespace Rotor;

//------------------------------------------------------------------------------  

Point
direction( const Vector & v1, const Vector & v2 )
{
  return v2.origin() - v1.origin();
}

//------------------------------------------------------------------------------  

void 
computePathAngles( Path & path )
{
  path[0].direction( direction( path[0], path[1] ) );
  size_t i;
  for ( i = 1; i < path.size() - 1; ++i ) {
    path[i].direction(  0.5 * (   direction( path[i-1], path[i] ) 
                                + direction( path[i], path[i+1] ) ) );
  }
  path[i].direction( direction( path[i-1], path[i] ) );
}

//------------------------------------------------------------------------------  

void
readPath( 
  const string & filename, 
  double samplingDistance, 
  Path & path )
{
  path.clear();
 ifstream in( filename.c_str() );
  string s;
  while ( getline( in, s ) ) { 
    stringstream line( s );
    path.resize( path.size() + 1 );
    Point & p = path.back().origin();
    line >> p[0];
    line >> p[1];
  }
//   resample( path, samplingDistance );
  computePathAngles( path );
}

//------------------------------------------------------------------------------

void
mainLoop( Registry & registry, ArcController & controller, double velocity )
{
  Structure commandStructure( "smart_velocity_message", 0, registry );
  commandStructure["host"]   = const_cast<char*>( hostName().c_str() );
  smart_velocity_message & command = ROTOR_VARIABLE( smart_velocity_message, commandStructure );

// 
// points          = []
// pose            = wp2.geometry.Vector( ( 0, 0 ), 0 )
  double steeringAngle   = 0;
  double actualSteering  = 0;
  double appliedVelocity = velocity;
// 
  Logger::info( "Starting main loop", "pathFollow" );
  while ( true ) {
    try
    {
      Message msg      = registry.receiveMessage( 1 );
      Structure & data = msg.data();
      if ( msg.name() == "carmen_localize_globalpos" )
      {
        Point tmp;
        tmp[0] = data["globalpos"]["x"];
        tmp[1] = data["globalpos"]["y"];
        Vector pose( tmp, data["globalpos"]["theta"] );
        steeringAngle = controller.step( pose );
//         next.x = nextPoint.origin[0]
//         next.y = nextPoint.origin[1]
//         next.timestamp = rotorc.seconds()*/
        Logger::spam( "Received pose " + toString( pose.origin()[0] )
                                 + " " + toString( pose.origin()[1] ), "pathFollow" );
      } else if ( msg.name() == "smart_status_message" ) {
        actualSteering = data["steering_angle"];
      } else if ( msg.name() == "axt_message" ) {
  //       points  = []
  //       dx      = data.x
  //       dy      = data.y
  //       channel = data.channel
        for ( int i = 0; i < data["num_points"]; ++i )
        {
//           if ( channel[i] == 2 )
//           {
  //           points.append( ( dx[i], dy[i] ) )
//           }
        }
      }
      
  //     appliedVelocity = safety.step( velocity, actualSteering, points )
      
      command.timestamp = seconds();
      if ( controller.finished() )
      {
        command.tv = 0.0;
        command.steering_angle = 0;
      } else {
        command.tv = appliedVelocity;
        command.steering_angle = steeringAngle;
      }
      
      cout << commandStructure.toString();
      registry.sendStructure( "smart_velocity_message", commandStructure );
//       registry.sendStructure( "next_waypoint_message", next );
      
      if ( controller.finished() )
      {
        Logger::info( "Goal has been reached, shutting down", "pathFollow" );
        break;
      }
    } catch( MessagingTimeout ) {
      Logger::spam( "Timeout waiting for message", "pathFollow" );
      command.tv = 0;
      command.steering_angle = 0;
      registry.sendStructure( "smart_velocity_message", commandStructure   );
    }
  }
}

//------------------------------------------------------------------------------

void 
registerMessages( Registry & registry )
{
  registry.registerType( ROTOR_DEFINITION_STRING( carmen_point_t ) );
  
  registry.registerMessageType( 
    "carmen_localize_globalpos", 
    ROTOR_DEFINITION_STRING( carmen_localize_globalpos_message )
  );
  registry.subscribeToMessage( "carmen_localize_globalpos", true );

  registry.registerMessageType( 
    "axt_message", 
    ROTOR_DEFINITION_STRING( axt_message )
  );
  registry.subscribeToMessage( "axt_message", true );

  registry.registerMessageType( 
    "path_message", 
    ROTOR_DEFINITION_STRING( path_message )
  );
  
  registry.registerMessageType( 
    "smart_velocity_message", 
    ROTOR_DEFINITION_STRING( smart_velocity_message ) 
  );
}

//------------------------------------------------------------------------------

void
sendPathMessage( Registry & registry, const Path & path )
{
  Structure pathMessage( "path_message", 0, registry );
  pathMessage["point_count"] = static_cast<int>( path.size() );
  pathMessage.adjust();
  for ( size_t i = 0; i < path.size(); ++i )
  {
    pathMessage["x"][i]     = path[i].origin()[0];
    pathMessage["y"][i]     = path[i].origin()[1];
    pathMessage["theta"][i] = path[i].angle();
  }
  pathMessage["timestamp"] = seconds();
  registry.sendStructure( "path_message", pathMessage );
  Logger::spam( "Path message has been sent:" + pathMessage.toString(), "pathFollow" );
}

//------------------------------------------------------------------------------

int main( int argc, char * argv[] )
{
  Logger::setLevel( Logger::SPAM, "ArcController" );
  if ( argc != 2 )
  {
    cout << "Usage: " << argv[0] << " <config.ini>\n";
    exit( 1 );
  }
  
  string moduleName( argv[0] );
  
  BaseOptions options;
  options.fromString( fileContents( argv[1] ) );
  RemoteRegistry registry( "CarmenRegistry", moduleName, options, "lib" );
  registerMessages( registry );
  
  double velocity          = options.getDouble( moduleName, "velocity" );
  double orientationWeight = options.getDouble( moduleName, "orientationWeight" );
  double waypointSpacing   = options.getDouble( moduleName, "waypointSpacing" );
  int lookahead            = options.getInt( moduleName, "lookahead" );
  int maxLookahead         = options.getInt( moduleName, "maxLookahead" );
  double angleThreshold    = options.getDouble( moduleName, "angleThreshold" );

  double axesDistance      = options.getDouble( "smart", "axesDistance" );
  double laserDistance     = options.getDouble( "smart", "laserDistance" );
  double securityDistance  = options.getDouble( "smart", "securityDistance" );
  double wheelDistance     = options.getDouble( "smart", "wheelDistance" );

  
  Path path;
  readPath( "path.txt", waypointSpacing, path );
  
  ArcController controller(
    path, axesDistance, orientationWeight, 
    lookahead, maxLookahead, angleThreshold,
    false
  );
  
  mainLoop( registry, controller, velocity );

// safety     = wp2.ArcSafety( axesDistance, laserDistance, securityDistance, wheelDistance )

}