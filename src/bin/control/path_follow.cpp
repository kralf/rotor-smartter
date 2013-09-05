#include "ArcController.h"
#include "ArcSafety.h"
#include "Vector.h"

#include <rotor/BaseOptions.h>
#include <rotor/Conversion.h>
#include <rotor/FileUtils.h>
#include <rotor/Logger.h>
#include <rotor/NetUtils.h>
#include <rotor/RemoteRegistry.h>
#include <rotor/Time.h>
#include <rotor/Thread.h>

#include <string>
#include <sstream>
#include <fstream>
#include <signal.h>

#include "Messages.h"

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
  std::vector<unsigned char> laserStatus;

  double steeringAngle   = 0;
  double actualSteering  = 0;
  double appliedVelocity = velocity;

  double intervalStart = 0.0;
  double cycleStart = 0.0;
  size_t numCycles = 0;

  while ( !quit )
  {
    double now = seconds();
    cycleStart = now;

    bool step = false;
    bool sent = false;

    while ( !sent && !quit )
    {
      now = seconds();
      intervalStart = ( intervalStart > 0.0 ) ? intervalStart : now;

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

          // cout << data.toString() << endl;
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

            pair<double, double> command = controller.step( pose );
            steeringAngle = command.first;
            velocity = command.second;
            step = true;

            Logger::spam( "Received pose " + toString( pose.origin()[0] ) +
              " " + toString( pose.origin()[1] ), "pathFollow" );
          } else if ( msg.name() == "carmen_base_odometry" ) {
            Point tmp;

            tmp[0] = data["x"];
            tmp[1] = data["y"];
            Vector pose( tmp, data["theta"] );

            pair<double, double> command = controller.step( pose );
            steeringAngle = command.first;
            velocity = command.second;
            step = true;

            Logger::spam( "Received pose " + toString( pose.origin()[0] ) +
              " " + toString( pose.origin()[1] ), "pathFollow" );
          } else if ( msg.name() == "locfilter_filteredpos_message" ) {
            Point tmp;

            tmp[0] = data["filteredpos"]["x"];
            tmp[1] = data["filteredpos"]["y"];
            Vector pose( tmp, data["filteredpos"]["theta"] );

            pair<double, double> command = controller.step( pose );
            steeringAngle = command.first;
            velocity = command.second;
            step = true;

            Logger::spam( "Received pose " + toString( pose.origin()[0] ) +
              " " + toString( pose.origin()[1] ), "pathFollow" );
          } else if ( msg.name() == "smart_status_message" ) {
            actualSteering = data["steering_angle"];
          } else if ( msg.name() == "axt_message" ) {
            axt_message & alasca = ROTOR_VARIABLE( axt_message, data );

            laserX.clear();
            laserY.clear();
            laserStatus.clear();

            for ( size_t i = 0; i < alasca.num_points; ++i ) {
              laserX.push_back( alasca.x[i] );
              laserY.push_back( alasca.y[i] );
              laserStatus.push_back( alasca.point_status[i] );
            }

            step = true;
          }

          if ( controller.finished() )
          {
            sendCommandMessage( registry, 0, 0 );
            sendStatusMessage( registry, Point(0, 0), idle );
            Logger::info( "Goal has been reached, going into idle mode",
              "pathFollow" );

            sent = true;
          } else if ( step ) {
            appliedVelocity = safety.step( velocity, actualSteering, laserX,
              laserY, laserStatus );

            sendCommandMessage( registry, appliedVelocity, steeringAngle );

            if ( appliedVelocity < velocity )
              sendStatusMessage( registry, controller.current(), waiting );
            else
              sendStatusMessage( registry, controller.current(), following );

            sent = true;
          }
        }
        else {
          sendCommandMessage( registry, 0, 0 );
          sendStatusMessage( registry, Point(0, 0), idle );

          sent = true;
        }
      } catch( MessagingTimeout ) {
        sendCommandMessage( registry, 0, 0 );
        Logger::spam( "Timeout waiting for message", "pathFollow" );

        sent = true;
      }

      now = seconds();
      if ( ( now - intervalStart ) >= updateInterval ) {
        fprintf( stderr, "Update frequency is %4.2f Hz\n",
          numCycles / ( now - intervalStart ) );

        numCycles     = 0;
        intervalStart = now;
      }
      else if ( sent )
        numCycles++;
    }

    now = seconds();
    if ( ( now - cycleStart ) < ( 1.0 / maxControlFrequency ) )
      Thread::sleep( ( 1.0 / maxControlFrequency - ( now - cycleStart ) ) );
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
  } else if ( localizationMessage == "carmen_base_odometry" ) {
    registry.registerMessageType(
      "carmen_base_odometry",
      ROTOR_DEFINITION_STRING( carmen_base_odometry_message )
    );
    registry.subscribeToMessage( "carmen_base_odometry", true );
  } else if ( localizationMessage == "locfilter_filteredpos_message" ) {
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
    "smart_status_message",
    ROTOR_DEFINITION_STRING( smart_status_message )
  );
  registry.subscribeToMessage( "smart_status_message", true );

  registry.registerMessageType(
    "smart_velocity_message",
    ROTOR_DEFINITION_STRING( smart_velocity_message )
  );
}

//------------------------------------------------------------------------------

int main( int argc, char * argv[] )
{
  Logger::setLevel( Logger::SPAM, "ArcController" );

  string command = "rotor-path_follow";
  if ( argc < 2 )
  {
    cout << "Usage: " << argv[0] << " <config.ini> [path.txt]\n";
    exit( 1 );
  }

  string moduleName( "path_follow" );

  BaseOptions options;
  options.fromString( fileContents( argv[1] ) );
  RemoteRegistry registry( "CarmenRegistry", moduleName, options );

  string localizationMessage = options.getString( moduleName,
    "localizationMessage" );
  registerMessages( registry, localizationMessage );

  double velocity            = options.getDouble( moduleName, "velocity" );
  double orientationWeight   = options.getDouble( moduleName, "orientationWeight" );
  double waypointSpacing     = options.getDouble( moduleName, "waypointSpacing" );
  int lookahead              = options.getInt( moduleName, "lookahead" );
  int maxLookahead           = options.getInt( moduleName, "maxLookahead" );
  double lookAheadThreshold  = options.getDouble( moduleName, "lookAheadThreshold" );
  double maxControlFrequency = options.getDouble( moduleName, "maxControlFrequency" );

  double deceleration  = options.getDouble( moduleName, "deceleration" );

  double securityDeceleration  = options.getDouble( moduleName, "securityDeceleration" );
  double securityMinDistance  = options.getDouble( moduleName, "securityMinDistance" );
  size_t securityMinHits   = options.getInt( moduleName, "securityMinHits" );

  double axesDistance      = options.getDouble( "smart", "axesDistance" );
  double laserDistance     = options.getDouble( "smart", "laserDistance" );
  double wheelDistance     = options.getDouble( "smart", "wheelDistance" );

  ArcController controller(
    axesDistance, orientationWeight,
    lookahead, maxLookahead, lookAheadThreshold,
    velocity, deceleration,
    false
  );

  ArcSafety safety(
    axesDistance,
    laserDistance,
    securityDeceleration,
    securityMinDistance,
    securityMinHits,
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
