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
  _window.mainWidget().frequencyPlot->setRegistry( registry );
  _window.mainWidget().frequencyPlot->addPlot( "smart_status_message", 150 );
  _window.mainWidget().frequencyPlot->addPlot( "carmen_base_odometry", 150 );
  _window.mainWidget().frequencyPlot->addPlot( "gyro_integrated_message", 120 );
  _window.mainWidget().frequencyPlot->addPlot( "locfilter_filteredpos_message", 120 );
  _window.mainWidget().frequencyPlot->addPlot( "axt_message", 30 );
}

//------------------------------------------------------------------------------

void
DispatchThread::run()
{
  while( true ) {
    Message message   = _registry.receiveMessage();
    Structure data    = message.data();
    if ( message.name() == "locfilter_filteredpos_message" ) {
      locfilter_filteredpos_message & filteredpos = ROTOR_VARIABLE( locfilter_filteredpos_message, data );
      _window.mainWidget().localizationPlot->updatePath( "Global", filteredpos.filteredpos.x, filteredpos.filteredpos.y );
    } else if ( message.name() == "carmen_base_odometry" ) {
      carmen_base_odometry_message & odometry = ROTOR_VARIABLE( carmen_base_odometry_message, data );
      _window.mainWidget().localizationPlot->updatePath( "Odometry", odometry.x, odometry.y );
      double steeringAngle = 0;
      if ( fabs( odometry.rv ) > 1E-6 && fabs( odometry.tv ) > 1E-6 )
      {
        double radius = odometry.tv / odometry.rv;
        int sign      = radius / fabs( radius );
        steeringAngle = sign * shortestAngle( atan2( _configuration.axesDistance(), fabs( radius ) ) );
      }
      _window.mainWidget().navigationPlot->steeringAngle( steeringAngle );
    } else if ( message.name() == "axt_message" ) {
        axt_message & alasca = ROTOR_VARIABLE( axt_message, data );
        _window.mainWidget().navigationPlot->resetLaserData();
        for ( size_t i = 0; i < alasca.num_points; ++i ) {
          if ( alasca.channel[i] == 2 ) {
            _window.mainWidget().navigationPlot->addLaserPoint( -alasca.y[i], alasca.x[i], alasca.point_status[i] );
          }
        }
    } else if ( message.name() == "smart_velocity_message" ) {
      smart_velocity_message & velocity = ROTOR_VARIABLE( smart_velocity_message, data );
      _window.mainWidget().navigationPlot->commandSteeringAngle( velocity.steering_angle );
    } else if (message.name() == "smart_status_message") {
      smart_status_message &status = ROTOR_VARIABLE(smart_status_message, data);
      _window.mainWidget().statusPlot->updateStatus(status.gas_pos, status.gear,
                                                    status.steering_angle,
                                                    status.tv,
                                                    status.rv_front_right,
                                                    status.rv_front_left,
                                                    status.rv_rear_right,
                                                    status.rv_rear_left);
    } else if (message.name() == "gyro_integrated_message") {
      gyro_integrated_message &gyro = ROTOR_VARIABLE(gyro_integrated_message, data);
      _window.mainWidget().gyroPlot->updateGyro(gyro.theta);
    }
  }

}
