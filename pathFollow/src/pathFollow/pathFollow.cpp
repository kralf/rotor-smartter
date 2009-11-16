#include "ArcController.h"
#include "ArcSafety.h"
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
#include <signal.h>

using namespace std;
using namespace Rotor;

const double updateInterval = 5.0;

bool quit = false;

//------------------------------------------------------------------------------

void quitLoop(int q __attribute__((unused))) {
  quit = true;
}

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

  computePathAngles( path );
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
  Logger::spam( "Path message has been sent:" + pathMessage.toString(),
    "pathFollow" );
}

//------------------------------------------------------------------------------

void
sendCommandMessage(
  Registry & registry,
  double tv,
  double steering_angle
)
{
  Structure commandStructure( "smart_velocity_message", 0, registry );
  commandStructure["host"] = const_cast<char*>( hostName().c_str() );
  smart_velocity_message & command = ROTOR_VARIABLE(smart_velocity_message,
    commandStructure );

  command.tv = tv;
  command.steering_angle = steering_angle;
  command.timestamp = seconds();

  // cout << commandStructure.toString();
  registry.sendStructure( "smart_velocity_message", commandStructure );
}

//------------------------------------------------------------------------------

void
sendStatusMessage(
  Registry & registry,
  const Point & nextPoint,
  State state
)
{
  Structure statusStructure( "path_status_message", 0, registry );
  statusStructure["host"] = const_cast<char*>( hostName().c_str() );
  path_status_message & status = ROTOR_VARIABLE(path_status_message,
    statusStructure );

  status.x = nextPoint[0];
  status.y = nextPoint[1];
  status.state = state;

  status.timestamp = seconds();

  // cout << statusStructure.toString();
  registry.sendStructure( "path_status_message", statusStructure );
}

//------------------------------------------------------------------------------

void
mainLoop( Registry & registry, ArcController & controller, ArcSafety & safety,
  double velocity, double maxControlFrequency )
{
  std::vector<double> laserX;
  std::vector<double> laserY;

  double steeringAngle   = 0;
  double actualSteering  = 0;
  double appliedVelocity = velocity;

  double intervalStart = 0.0;
  double cycleStart = 0.0;
  size_t numCycles = 0;

  while ( !quit ) {
    double now           = seconds();
    intervalStart = ( intervalStart > 0.0 ) ? intervalStart : now;
    cycleStart    = now;

    bool step = false;

    while ( !step && !quit )
    {
      try
      {
        Message msg = registry.receiveMessage( 1 );

        if ( msg.name() == "path_message" )
        {
          Structure & data = msg.data();
          Path path;
          path_message & pathMessage = * reinterpret_cast<path_message *>(
            data.buffer() );

          path.resize( pathMessage.point_count );
          for ( size_t i = 0; i < pathMessage.point_count; ++i )
          {
            Point & p = path[i].origin();
            p[0] = pathMessage.x[i];
            p[1] = pathMessage.y[i];
          }

          computePathAngles( path );
          controller.path( path );

          Logger::spam( "Path message has been received:" + data.toString(),
            "pathFollow" );
        } else if ( msg.name() == "path_stop_message" ) {
          Structure & data = msg.data();

          controller.path( Path() );

          Logger::spam( "Stop message has been received:" + data.toString(),
            "pathFollow" );
        } else if ( !controller.finished() ) {
          Structure & data = msg.data();

          if ( msg.name() == "carmen_localize_globalpos" )
          {
            Point tmp;

            tmp[0] = data["globalpos"]["x"];
            tmp[1] = data["globalpos"]["y"];
            Vector pose( tmp, data["globalpos"]["theta"] );

            steeringAngle = controller.step( pose );
            step = true;

            Logger::spam( "Received pose " + toString( pose.origin()[0] ) +
              " " + toString( pose.origin()[1] ), "pathFollow" );
          } else if ( msg.name() == "locfilter_filteredpos_message" ) {
            Point tmp;

            tmp[0] = data["filteredpos"]["x"];
            tmp[1] = data["filteredpos"]["y"];
            Vector pose( tmp, data["filteredpos"]["theta"] );

            steeringAngle = controller.step( pose );
            step = true;

            Logger::spam( "Received pose " + toString( pose.origin()[0] ) +
              " " + toString( pose.origin()[1] ), "pathFollow" );
          } else if ( msg.name() == "smart_status_message" ) {
            actualSteering = data["steering_angle"];
          } else if ( msg.name() == "axt_message" ) {
            axt_message & alasca = ROTOR_VARIABLE( axt_message, data );

            laserX.clear();
            laserY.clear();

            for ( size_t i = 0; i < alasca.num_points; ++i ) {
              if ( alasca.channel[i] == 2 ) {
                laserX.push_back( alasca.x[i] );
                laserY.push_back( -alasca.y[i] );
              }
            }
          }

          appliedVelocity = safety.step( velocity, actualSteering, laserX, laserY );

          if ( controller.finished() )
          {
            sendCommandMessage( registry, 0, 0 );
            sendStatusMessage( registry, Point(0, 0), idle );
            Logger::info( "Goal has been reached, going into idle mode",
              "pathFollow" );
          } else {
            sendCommandMessage( registry, appliedVelocity, steeringAngle );

            if ( appliedVelocity == 0 )
              sendStatusMessage( registry, controller.current(), waiting );
            else
              sendStatusMessage( registry, controller.current(), following );
        }
        }
        else {
          sendCommandMessage( registry, 0, 0 );
          sendStatusMessage( registry, Point(0, 0), idle );
        }
      } catch( MessagingTimeout ) {
        sendCommandMessage( registry, 0, 0 );
        Logger::spam( "Timeout waiting for message", "pathFollow" );
      }

      now = seconds();

      if ( ( now - cycleStart ) < ( 1.0 / maxControlFrequency ) )
        usleep( ( 1.0 / maxControlFrequency - ( now - cycleStart ) ) * 1e6 );

      now = seconds();

      if ( ( now - intervalStart ) >= updateInterval ) {
        fprintf( stderr, "Update frequency is %4.2f Hz\n",
          numCycles / ( now - intervalStart ) );

        numCycles     = 0;
        intervalStart = now;
      }
      else
        numCycles++;
    }
  }
}

//------------------------------------------------------------------------------

void
registerMessages( Registry & registry, const string & localizationMessage )
{
  registry.registerType( ROTOR_DEFINITION_STRING( carmen_point_t ) );

  if ( localizationMessage == "carmen_localize_globalpos" )
  {
    registry.registerMessageType(
      "carmen_localize_globalpos",
      ROTOR_DEFINITION_STRING( carmen_localize_globalpos_message )
    );
    registry.subscribeToMessage( "carmen_localize_globalpos", true );
  }
  else if ( localizationMessage == "locfilter_filteredpos_message" )
  {
    registry.registerMessageType(
      "locfilter_filteredpos_message",
      ROTOR_DEFINITION_STRING( locfilter_filteredpos_message )
    );
    registry.subscribeToMessage( "locfilter_filteredpos_message", true );
  }

  registry.registerMessageType(
    "axt_message",
    ROTOR_DEFINITION_STRING( axt_message )
  );
  registry.subscribeToMessage( "axt_message", true );

  registry.registerMessageType(
    "path_message",
    ROTOR_DEFINITION_STRING( path_message )
  );
  registry.subscribeToMessage( "path_message", true );

  registry.registerMessageType(
    "path_stop_message",
    ROTOR_DEFINITION_STRING( path_stop_message )
  );
  registry.subscribeToMessage( "path_stop_message", true );

  registry.registerMessageType(
    "path_status_message",
    ROTOR_DEFINITION_STRING( path_status_message )
  );

  registry.registerMessageType(
    "smart_velocity_message",
    ROTOR_DEFINITION_STRING( smart_velocity_message )
  );
}

//------------------------------------------------------------------------------

int main( int argc, char * argv[] )
{
  Logger::setLevel( Logger::SPAM, "ArcController" );
  if ( argc < 2 )
  {
    cout << "Usage: " << argv[0] << " <config.ini> [path.txt]\n";
    exit( 1 );
  }

  string moduleName( argv[0] );

  BaseOptions options;
  options.fromString( fileContents( argv[1] ) );
  RemoteRegistry registry( "CarmenRegistry", moduleName, options, "lib" );

  string localizationMessage = options.getString( moduleName,
    "localizationMessage" );
  registerMessages( registry, localizationMessage );

  double velocity            = options.getDouble( moduleName, "velocity" );
  double orientationWeight   = options.getDouble( moduleName, "orientationWeight" );
  double waypointSpacing     = options.getDouble( moduleName, "waypointSpacing" );
  int lookahead              = options.getInt( moduleName, "lookahead" );
  int maxLookahead           = options.getInt( moduleName, "maxLookahead" );
  double angleThreshold      = options.getDouble( moduleName, "angleThreshold" );
  double maxControlFrequency = options.getDouble( moduleName, "maxControlFrequency" );


  double axesDistance      = options.getDouble( "smart", "axesDistance" );
  double laserDistance     = options.getDouble( "smart", "laserDistance" );
  double securityDistance  = options.getDouble( "smart", "securityDistance" );
  double wheelDistance     = options.getDouble( "smart", "wheelDistance" );

  ArcController controller(
    axesDistance, orientationWeight,
    lookahead, maxLookahead, angleThreshold,
    false
  );

  ArcSafety safety(
    axesDistance,
    laserDistance,
    securityDistance,
    wheelDistance
  );

  if ( argc == 3 )
  {
    Path path;
    readPath( argv[2], waypointSpacing, path );

    controller.path( path );
  }

  signal( SIGINT, quitLoop );

  mainLoop( registry, controller, safety, velocity, maxControlFrequency );

  sendCommandMessage( registry, 0, 0 );
  sendStatusMessage( registry, Point(0, 0), idle );
}
