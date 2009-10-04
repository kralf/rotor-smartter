#include "DispatchThread.h"
#include "ApplicationWindow.h"
#include <smart-rotor-interfaces/Messages.h>
#include <rotor/Structure.h>
#include <rotor/Logger.h>

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
      carmen_localize_globalpos_message & global = ROTOR_VARIABLE( carmen_localize_globalpos_message, data );
      _window.mainWidget().localizationPlot->updatePath( "Global", global.globalpos.x, global.globalpos.y );
    } else if ( message.name() == "carmen_base_odometry" ) {
      carmen_base_odometry_message & odometry = ROTOR_VARIABLE( carmen_base_odometry_message, data );
      _window.mainWidget().localizationPlot->updatePath( "Odometry", odometry.x, odometry.y );
    } else if ( message.name() == "axt_message" ) {
        axt_message & alasca = ROTOR_VARIABLE( axt_message, data );
        _window.mainWidget().navigationPlot->resetLaserData();
        for ( size_t i = 0; i < alasca.num_points; ++i ) {
          if ( alasca.channel[i] == 2 ) {
            _window.mainWidget().navigationPlot->addLaserPoint( -alasca.y[i], alasca.x[i] );
          }
        }
    }
  }

}
