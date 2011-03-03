#include <rotor/Structure.h>
#include <rotor/Logger.h>

#include "DispatchThread.h"
#include "ApplicationWindow.h"

#include "Messages.h"

#include "ui_MainWindow.h"

using namespace Rotor;

//------------------------------------------------------------------------------

DispatchThread::DispatchThread(
  Registry & registry,
  ApplicationWindow & window
)
  : _registry( registry ),
    _window( window )
{
}

//------------------------------------------------------------------------------

void
DispatchThread::run()
{
  while( true ) {
    Message message   = _registry.receiveMessage();
    Structure data    = message.data();
    if ( message.name() == "carmen_localize_globalpos" ) {
      carmen_localize_globalpos_message & globalpos = ROTOR_VARIABLE(
        carmen_localize_globalpos_message, data );
      _window.mainWidget().pathPlot->setPose( globalpos.globalpos.x,
        globalpos.globalpos.y, globalpos.globalpos.theta );
    } else if ( message.name() == "locfilter_filteredpos_message" ) {
      locfilter_filteredpos_message & filteredpos = ROTOR_VARIABLE(
        locfilter_filteredpos_message, data );
      _window.mainWidget().pathPlot->setPose( filteredpos.filteredpos.x,
        filteredpos.filteredpos.y, filteredpos.filteredpos.theta );
    } else if ( message.name() == "carmen_base_odometry" ) {
      carmen_base_odometry_message & odometry = ROTOR_VARIABLE(
        carmen_base_odometry_message, data );
      _window.mainWidget().pathPlot->setPose( odometry.x, odometry.y,
        odometry.theta );
    } else if ( message.name() == "path_message" ) {
     _window.mainWidget().pathPlot->clearPath();
      path_message & path = ROTOR_VARIABLE( path_message, data );
      for ( size_t i = 0; i < path.point_count; ++i )
        _window.mainWidget().pathPlot->updatePath( path.x[i], path.y[i] );
    } else if ( message.name() == "path_stop_message" ) {
      _window.mainWidget().pathPlot->clearPath();
    } else if ( message.name() == "path_status_message" ) {
      path_status_message & status = ROTOR_VARIABLE( path_status_message,
        data );
    }
  }
}
