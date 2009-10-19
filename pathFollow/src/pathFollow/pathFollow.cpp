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

void
readPath( 
  const string & filename, 
  double samplingDistance, 
  ArcController::Path & path )
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
//   computeAngles( path );
}

// #-------------------------------------------------------------------------------  
// 
// def steeringAngleToRv( axesDistance, tv, steeringAngle ):
//   sign   = -steeringAngle / abs( steeringAngle  )
//   radius = sign * sqrt( ( axesDistance / sin( steeringAngle ) )**2 - axesDistance**2 )
//   rotorc.Logger.info( "Radius %f" % radius )
//   rv     = tv / radius
//   return rv
//   
//   
// #-------------------------------------------------------------------------------  
// 
// moduleName = os.path.basename( sys.argv[0] )
// if len( sys.argv ) != 2:
//   rotorc.Logger.error( "Usage: " + moduleName + " <config.ini>" )
//   exit( 1 )
//   
// options = rotorc.BaseOptions()
// options.fromString( rotorc.fileContents( sys.argv[1] ) );
// registry  = rotorc.RemoteRegistry( "CarmenRegistry", moduleName, options, "lib" );
// 
// 
// globalLocalizationMessage = options.getString( moduleName, "globalLocalizationMessage" )
// velocity                  = options.getDouble( moduleName, "velocity" )
// orientationWeight         = options.getDouble( moduleName, "orientationWeight" )
// waypointSpacing           = options.getDouble( moduleName, "waypointSpacing" )
// lookahead                 = options.getInt( moduleName, "lookahead" )
// maxLookahead              = options.getInt( moduleName, "maxLookahead" )
// angleThreshold            = options.getDouble( moduleName, "angleThreshold" )
// 
// axesDistance              = options.getDouble( "smart", "axesDistance" )
// laserDistance             = options.getDouble( "smart", "laserDistance" )
// securityDistance          = options.getDouble( "smart", "securityDistance" )
// wheelDistance             = options.getDouble( "smart", "wheelDistance" )
// 
// 
// 
// registry.registerType( carmen_point_t )
// registry.registerMessageType( globalLocalizationMessage, carmen_localize_globalpos_message )
// registry.subscribeToMessage( globalLocalizationMessage, True );
// 
// registry.registerMessageType( "axt_message", axt_message )
// registry.subscribeToMessage( "axt_message", True );
// 
// registry.registerMessageType( "path_message", path_message )
// registry.registerMessageType( "carmen_base_velocity_message", carmen_base_velocity_message )
// 
// command          = registry.newStructure( "carmen_base_velocity_message" )
// command.host     = socket.gethostname()
// pathMessage      = registry.newStructure( "path_message" )
// pathMessage.host = socket.gethostname()
// 
// #-------------------------------------------------------------------------------  
// 
// path       = readPath( "path.txt", waypointSpacing )
// 
// pathMessage.point_count = len( path )
// pathMessage.adjust()
// for i in xrange( len( path ) ):
//   pathMessage.x[i]     = path[i].origin[0]
//   pathMessage.y[i]     = path[i].origin[1]
//   pathMessage.theta[i] = path[i].angle()
// pathMessage.timestamp = rotorc.seconds()
// registry.sendStructure( "path_message", pathMessage )
// rotorc.Logger.spam( "Path message has been sent:" + pathMessage.toString() )
// 
// controller = wp2.ArcController( path, axesDistance, orientationWeight, 
//   cycle = False, 
//   lookahead = lookahead, maxLookahead = maxLookahead, 
//   angleThreshold = angleThreshold 
// )
// safety     = wp2.ArcSafety( axesDistance, laserDistance, securityDistance, wheelDistance )
// 
// points          = []
// pose            = wp2.geometry.Vector( ( 0, 0 ), 0 )
// steeringAngle   = 0
// actualSteering  = 0
// appliedVelocity = velocity
// 
// rotorc.Logger.info( "Starting main loop" )
// 
// 
// while True:
//   try:
//     msg  = registry.receiveMessage( 1 )
//     data = msg.data()
//     if msg.name() == globalLocalizationMessage:
//       pose = wp2.geometry.Vector( ( data.globalpos.x, data.globalpos.y ), data.globalpos.theta )
//       nextPoint, steeringAngle = controller.step( pose )
//       next.x = nextPoint.origin[0]
//       next.y = nextPoint.origin[1]
//       next.timestamp = rotorc.seconds()
//       rotorc.Logger.spam( "Received pose %f, %f" % ( pose.origin[0], pose.origin[1] ) )
//     elif msg.name() == "smart_status_message":
//       actualSteering = data.steering_angle
//     elif msg.name() == "axt_message":
//       points  = []
//       dx      = data.x
//       dy      = data.y
//       channel = data.channel
//       for i in range( data.num_points ):
//         if channel[i] == 2:
//           points.append( ( dx[i], dy[i] ) )
//     
//     appliedVelocity = safety.step( velocity, actualSteering, points )
//     
//     command.timestamp = rotorc.seconds()
//     if controller.finished():
//       command.tv = 0.0
//       command.rv = 0
//     else:
//       command.tv = appliedVelocity
//       command.rv = steeringAngleToRv( axesDistance, command.tv, steeringAngle )
//     
//     registry.sendStructure( "carmen_base_velocity", command )
//     registry.sendStructure( "next_waypoint_message", next )
//     
//     if controller.finished():
//       rotorc.Logger.info( "Goal has been reached, shutting down. Goal(%f, %f) Pose(%f, %f)" % ( path[-1].origin[0], path[-1].origin[1], pose.origin[0], pose.origin[1] ) )
//       break
//   except rotorc.MessagingTimeout:
//     rotorc.Logger.spam( "Timeout waiting for message" )
//     command.tv = 0
//     command.rv = 0
//     registry.sendStructure( "carmen_base_velocity", command )


//------------------------------------------------------------------------------

void
mainLoop( Registry & registry, ArcController & controller, double velocity )
{
  Structure command( "carmen_base_velocity_message", 0, registry );
  command["host"]   = const_cast<char*>( hostName().c_str() );

// safety     = wp2.ArcSafety( axesDistance, laserDistance, securityDistance, wheelDistance )
// 
// points          = []
// pose            = wp2.geometry.Vector( ( 0, 0 ), 0 )
// steeringAngle   = 0
  double actualSteering  = 0;
  double appliedVelocity = velocity;
// 
  Logger::info( "Starting main loop", "pathFollow" );
  while ( true ) {
    try
    {
      Message msg      = registry.receiveMessage( 1 );
      Structure & data = msg.data();
      if ( msg.name() == "carmen_localize_globalpos_message" )
      {
        Point tmp;
        tmp[0] = data["globalpos"]["x"];
        tmp[1] = data["globalpos"]["y"];
        Vector pose( tmp, data["globalpos"]["theta"] );
  /*      nextPoint, steeringAngle = controller.step( pose )
        next.x = nextPoint.origin[0]
        next.y = nextPoint.origin[1]
        next.timestamp = rotorc.seconds()*/
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
      
      command["timestamp"] = seconds();
      if ( controller.finished() )
      {
        command["tv"] = 0.0;
        command["rv"] = 0;
      } else {
        command["tv"] = appliedVelocity;
  //       command["rv"] = steeringAngleToRv( axesDistance, command["tv"], steeringAngle );
      }
      
      registry.sendStructure( "carmen_base_velocity", command );
//       registry.sendStructure( "next_waypoint_message", next );
      
      if ( controller.finished() )
      {
        Logger::info( "Goal has been reached, shutting down", "pathFollow" );
        break;
      }
    } catch( MessagingTimeout ) {
      Logger::spam( "Timeout waiting for message", "pathFollow" );
      command["tv"] = 0;
      command["rv"] = 0;
      registry.sendStructure( "carmen_base_velocity", command );
    }
  }
}

//------------------------------------------------------------------------------

void 
registerMessages( Registry & registry )
{
  registry.registerType( ROTOR_DEFINITION_STRING( carmen_point_t ) );
  
  registry.registerMessageType( 
    "carmen_localize_globalpos_message", 
    ROTOR_DEFINITION_STRING( carmen_localize_globalpos_message )
  );
  registry.subscribeToMessage( "carmen_localize_globalpos_message", true );

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
    "carmen_base_velocity_message", 
    ROTOR_DEFINITION_STRING( carmen_base_velocity_message )
  );
}

//------------------------------------------------------------------------------

void
sendPathMessage( Registry & registry, const ArcController::Path & path )
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

  
  ArcController::Path path;
  readPath( "path.txt", waypointSpacing, path );
  
  ArcController controller(
    path, axesDistance, orientationWeight, 
    lookahead, maxLookahead, angleThreshold,
    false
  );
  
}