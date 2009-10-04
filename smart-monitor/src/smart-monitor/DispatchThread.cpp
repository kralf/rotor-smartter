#include "DispatchThread.h"
#include "ApplicationWindow.h"
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
      Logger::info( data.toString() );
      double x       = data["globalpos"]["x"];
      double y       = data["globalpos"]["y"];
//       double theta   = data["globalpos"]["theta"];
      _window.mainWidget().localizationPlot->updatePath( "Global", x, y );
    } else if ( message.name() == "carmen_base_odometry" ) {
      Logger::info( data.toString() );
      double x       = data["x"];
      double y       = data["y"];
//       double theta   = data["theta"];
      _window.mainWidget().localizationPlot->updatePath( "Odometry", x, y );
    } else if ( message.name() == "axt_message" ) {
        double dx      = data["x"];
        double dy      = data["y"];
        int channel    = data["channel"];
        size_t count   = data["num_points"];
        _window.minWidget().navigationPlot->resetLaserData();
        for ( size_t i = 0; i < count; ++i ) {
          if ( channel == 2 ) {
            _window.minWidget().navigationPlot->addLaserPoint( ( -dy[i], dx[i] ) );
          }
        }
    }
  }

}
