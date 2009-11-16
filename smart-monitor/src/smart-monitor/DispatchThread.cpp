#include "DispatchThread.h"
#include "ApplicationWindow.h"
#include "Configuration.h"
#include <smart-rotor-interfaces/Messages.h>
#include <rotor/Structure.h>
#include <rotor/Logger.h>

using namespace Rotor;

//------------------------------------------------------------------------------

double shortestAngle( double angle )
{
  while ( angle > M_PI )
  {
    angle = angle - M_PI;
  }
  while ( angle < -M_PI )
  {
    angle = angle + M_PI;
  }
  return angle;
}

//------------------------------------------------------------------------------

DispatchThread::DispatchThread(
  Registry & registry,
  ApplicationWindow & window
)
  : _registry( registry ),
    _window( window ),
    _configuration( window.configuration() )
{
  std::string defaultPath = _registry.options().getString( "smartMonitor",
    "defaultPath" );

  _window.mainWidget().localizationPlot->setRegistry( registry );
  _window.mainWidget().localizationPlot->setDefaultPath( defaultPath );

  _window.mainWidget().navigationPlot->setRegistry( registry );
  _window.mainWidget().navigationPlot->setDefaultPath( defaultPath );

  _window.mainWidget().frequencyPlot->setRegistry( registry );
  _window.mainWidget().frequencyPlot->addPlot( "smart_status_message", 175 );
  _window.mainWidget().frequencyPlot->addPlot( "carmen_base_odometry", 175 );

  _window.mainWidget().frequencyPlot->addPlot( "gyro_integrated_message", 100 );

  _window.mainWidget().frequencyPlot->addPlot( "axt_message", 25 );
  _window.mainWidget().frequencyPlot->addPlot( "carmen_robot_frontlaser", 25 );

  _window.mainWidget().frequencyPlot->addPlot( "locfilter_filteredpos_message", 100 );
  _window.mainWidget().frequencyPlot->addPlot( "carmen_localize_globalpos", 20 );

  _window.mainWidget().frequencyPlot->addPlot( "smart_velocity_message", 100 );
}

//------------------------------------------------------------------------------

void
DispatchThread::run()
{
  while( true ) {
    Message message   = _registry.receiveMessage();
    Structure data    = message.data();
    if ( message.name() == "carmen_localize_globalpos_message" ) {
      carmen_localize_globalpos_message & globalpos = ROTOR_VARIABLE(
        carmen_localize_globalpos_message, data );
      _window.mainWidget().localizationPlot->updatePath( "Global",
        globalpos.globalpos.x, globalpos.globalpos.y );
    } else if ( message.name() == "locfilter_filteredpos_message" ) {
      locfilter_filteredpos_message & filteredpos = ROTOR_VARIABLE(
        locfilter_filteredpos_message, data );
      _window.mainWidget().localizationPlot->updatePath( "Filter",
        filteredpos.filteredpos.x, filteredpos.filteredpos.y );
    } else if ( message.name() == "carmen_base_odometry" ) {
      carmen_base_odometry_message & odometry = ROTOR_VARIABLE(
        carmen_base_odometry_message, data );
      _window.mainWidget().localizationPlot->updatePath( "Odometry",
        odometry.x, odometry.y );
    } else if ( message.name() == "path_message" ) {
      _window.mainWidget().localizationPlot->clearPath( "Path" );
      path_message & path = ROTOR_VARIABLE( path_message, data );
      for ( size_t i = 0; i < path.point_count; ++i )
        _window.mainWidget().localizationPlot->updatePath( "Path",
          path.x[i], path.y[i] );
    } else if ( message.name() == "axt_message" ) {
      axt_message & alasca = ROTOR_VARIABLE( axt_message, data );
      _window.mainWidget().navigationPlot->resetLaserData();
      for ( size_t i = 0; i < alasca.num_points; ++i ) {
        if ( alasca.channel[i] == 2 ) {
          _window.mainWidget().navigationPlot->addLaserPoint(
            -alasca.y[i], alasca.x[i], alasca.point_status[i] );
        }
      }
    } else if ( message.name() == "smart_velocity_message" ) {
      smart_velocity_message & velocity = ROTOR_VARIABLE(
        smart_velocity_message, data );
      _window.mainWidget().controlPlot->updateControl(
          velocity.steering_angle, velocity.tv );
      _window.mainWidget().navigationPlot->commandSteeringAngle(
        velocity.steering_angle );
    } else if (message.name() == "smart_status_message") {
      smart_status_message &status = ROTOR_VARIABLE(
        smart_status_message, data );
      _window.mainWidget().statusPlot->updateStatus(
          status.gas_pos, status.gear, status.steering_angle,
          status.tv, status.rv_front_right, status.rv_front_left,
          status.rv_rear_right, status.rv_rear_left );
      _window.mainWidget().navigationPlot->steeringAngle(
        status.steering_angle );
    } else if (message.name() == "gyro_integrated_message") {
      gyro_integrated_message &gyro = ROTOR_VARIABLE(
        gyro_integrated_message, data );
      _window.mainWidget().gyroPlot->updateGyro( gyro.theta );
    }
  }
}
