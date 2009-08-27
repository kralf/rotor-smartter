#include "DispatchThread.h"
#include <rotor/Structure.h>

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

}
