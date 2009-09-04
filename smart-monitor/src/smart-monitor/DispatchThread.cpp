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
      _window.mainWidget().localizationPlot->updateGlobal( x, y );
    }
  }

}
